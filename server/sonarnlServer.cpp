/*
Adept MobileRobots Advanced Robotics Navigation and Localization (ARNL)
Version 1.7.5.1

Copyright (C) 2004, 2005 ActivMedia Robotics LLC
Copyright (C) 2006, 2007, 2008, 2009 MobileRobots Inc.
Copyright (C) 2010, 2011, 2012 Adept Technology, Inc.

All Rights Reserved.

Adept MobileRobots does not make any representations about the
suitability of this software for any purpose.  It is provided "as is"
without express or implied warranty.

The license for this software is distributed as LICENSE.txt in the top
level directory.

robots@mobilerobots.com
Adept MobileRobots
10 Columbia Drive
Amherst, NH 03031
800-639-9481

*/
/** @example sonarnlServer.cpp example of almost all SONARNL features */





/**
  This is an example of using ARNL and ArNetworking to provide a
  server program that remote clients, such as MobileEyes, can connect
  to, and which includes most of the features available in ARNL.

  You can make a copy of this program's source file in the 'examples' directory
  and use it as the basis for your own program.

  MobileEyes sends requests to this server program to go to goals,
  get the current map, set configuration parameters, manually relocalize, 
  request the current state of the robot and sensors, etc. This example server
  includes almost all features provided by ARNL (or SONARNL or MOGS), including
  localization, path planning and navigation to a map goal or abritrary point,
  sharing information with other servers peer-to-peer or through 
  a "central server", using IR and bumper sensors, stopping the robot
  if localization fails, global replanning if a path is not followable,
  use of special SICK laser reflector beacons, as well as various networking
  services for MobileEyes and other clients, such as diagnostic visualizations,
  access to configuration parameters, access to camera control and 
  video images (if SAVServer is running), special debugging commands 
  ("custom commands"), file uploading/downloading and gathering raw laser
  scans using Mapper3, and safe and unsafe teleoperation.
*/

#include <unistd.h>
#include <sstream>
#include <iostream>

#include "Aria.h"
#include "ArNetworking.h"
#include "Arnl.h"

#include "ArSonarLocalizationTask.h"

using namespace std;

/* Toggle Sonar localization */
#define SONAR_LOC 1

void logOptions(const char *progname)
{
  ArLog::log(ArLog::Normal, "Usage: %s [options]\n", progname);
  ArLog::log(ArLog::Normal, "[options] are any program options listed below, or any ARNL configuration");
  ArLog::log(ArLog::Normal, "parameters as -name <value>, see params/arnl.p for list.");
  ArLog::log(ArLog::Normal, "For example, -map <map file>.");
  Aria::logOptions();
}

bool gyroErrored = false;
const char* getGyroStatusString(ArRobot* robot)
{
  if(!robot || !robot->getOrigRobotConfig() || robot->getOrigRobotConfig()->getGyroType() < 2) return "N/A";
  if(robot->getFaultFlags() & ArUtil::BIT4)
  {
    gyroErrored = true;
    return "ERROR/OFF";
  }
  if(gyroErrored)
  {
    return "OK but error before";
  }
  return "OK";
}

int main(int argc, char **argv)
{
  // Initialize Aria and Arnl global information
  Aria::init();
  Arnl::init();


  // The robot object
  ArRobot robot;

  // Parse the command line arguments.
  ArArgumentParser parser(&argc, argv);

  // Set up our simpleConnector, to connect to the robot and laser
  //ArSimpleConnector simpleConnector(&parser);
  ArRobotConnector robotConnector(&parser, &robot);

  // Connect to the robot
  if (!robotConnector.connectRobot())
  {
    ArLog::log(ArLog::Normal, "Error: Could not connect to robot... exiting");
    Aria::exit(3);
  }



  // Set up where we'll look for files. Arnl::init() set Aria's default
  // directory to Arnl's default directory; addDirectories() appends this
  // "examples" directory.
  char fileDir[1024];
#if 0
  ArUtil::addDirectories(fileDir, sizeof(fileDir), Aria::getDirectory(), 
			 "examples");
#else
  stringstream ss;
  ss << get_current_dir_name();
  ss >> fileDir;
#endif
  cout << "fileDir = " << fileDir << endl;
  
  
  // To direct log messages to a file, or to change the log level, use these  calls:
  //ArLog::init(ArLog::File, ArLog::Normal, "log.txt", true, true);
  //ArLog::init(ArLog::File, ArLog::Verbose);
 
  // Add a section to the configuration to change ArLog parameters
  ArLog::addToConfig(Aria::getConfig());

  // set up a gyro (if the robot is older and its firmware does not
  // automatically incorporate gyro corrections, then this object will do it)
  ArAnalogGyro gyro(&robot);

  // Our networking server
  ArServerBase server;
  

  // Set up our simpleOpener, used to set up the networking server
  ArServerSimpleOpener simpleOpener(&parser);

  
  // Load default arguments for this computer (from /etc/Aria.args, environment
  // variables, and other places)
  parser.loadDefaultArguments();

  // Parse arguments 
  if (!Aria::parseArgs() || !parser.checkHelpAndWarnUnparsed())
  {
    logOptions(argv[0]);
    Aria::exit(1);
  }
  

  // This causes Aria::exit(9) to be called if the robot unexpectedly
  // disconnects
  ArGlobalFunctor1<int> shutdownFunctor(&Aria::exit, 9);
  robot.addDisconnectOnErrorCB(&shutdownFunctor);


  // Create an ArSonarDevice object (ArRangeDevice subclass) and 
  // connect it to the robot.
  ArSonarDevice sonarDev;
  robot.addRangeDevice(&sonarDev);



  // This object will allow robot's movement parameters to be changed through
  // a Robot Configuration section in the ArConfig global configuration facility.
  ArRobotConfig robotConfig(&robot);

  // Include gyro configuration options in the robot configuration section.
  robotConfig.addAnalogGyro(&gyro);

  // Start the robot thread.
  robot.runAsync(true);
  



    /* Create and set up map object */
  
  // Set up the map object, this will look for files in the examples
  // directory (unless the file name starts with a /, \, or .
  // You can take out the 'fileDir' argument to look in the program's current directory
  // instead.
  // When a configuration file is loaded into ArConfig later, if it specifies a
  // map file, then that file will be loaded as the map.
  ArMap map(fileDir);
  // set it up to ignore empty file names (otherwise if a configuration omits
  // the map file, the whole configuration change will fail)
  map.setIgnoreEmptyFileName(true);
  // ignore the case, so that if someone is using MobileEyes or
  // MobilePlanner from Windows and changes the case on a map name,
  // it will still work.
  map.setIgnoreCase(true);

    
    /* Create localization and path planning threads */


  ArPathPlanningTask pathTask(&robot, &sonarDev, &map);


  ArLog::log(ArLog::Normal, "Creating sonar localization task");
#if SONAR_LOC
  ArSonarLocalizationTask locTask(&robot, &sonarDev, &map);
#endif



    /* Start the server */

  // Open the networking server
  if (!simpleOpener.open(&server, fileDir, 240))
  {
    ArLog::log(ArLog::Normal, "Error: Could not open server.");
    exit(2);
  }



    /* Create various services that provide network access to clients (such as
     * MobileEyes), as well as add various additional features to ARNL */





  /* Add additional range devices to the robot and path planning task (so it
     avoids obstacles detected by these devices) */
  
  // Add IR range device to robot and path planning task (so it avoids obstacles
  // detected by this device)
  robot.lock();
  ArIRs irs;
  robot.addRangeDevice(&irs);
  pathTask.addRangeDevice(&irs, ArPathPlanningTask::CURRENT);

  // Add bumpers range device to robot and path planning task (so it avoids obstacles
  // detected by this device)
  ArBumpers bumpers;
  robot.addRangeDevice(&bumpers);
  pathTask.addRangeDevice(&bumpers, ArPathPlanningTask::CURRENT);

  // Add range device which uses forbidden regions given in the map to give virtual
  // range device readings to ARNL.  (so it avoids obstacles
  // detected by this device)
  ArForbiddenRangeDevice forbidden(&map);
  robot.addRangeDevice(&forbidden);
  pathTask.addRangeDevice(&forbidden, ArPathPlanningTask::CURRENT);

  robot.unlock();


#if SONAR_LOC
  // Action to slow down robot when localization score drops but not lost.
  ArActionSlowDownWhenNotCertain actionSlowDown(&locTask);
  pathTask.getPathPlanActionGroup()->addAction(&actionSlowDown, 140);

  // Action to stop the robot when localization is "lost" (score too low)
  ArActionLost actionLostPath(&locTask, &pathTask);
  pathTask.getPathPlanActionGroup()->addAction(&actionLostPath, 150);
#endif


  
  // Service to provide drawings of data in the map display :
  ArServerInfoDrawings drawings(&server);
  drawings.addRobotsRangeDevices(&robot);

  /* Draw a box around the local path planning area use this 
    (You can enable this particular drawing from custom commands 
    which is set up down below in ArServerInfoPath) */
  ArDrawingData drawingDataP("polyLine", ArColor(200,200,200), 1, 75);
  ArFunctor2C<ArPathPlanningTask, ArServerClient *, ArNetPacket *> 
    drawingFunctorP(&pathTask, &ArPathPlanningTask::drawSearchRectangle);
  drawings.addDrawing(&drawingDataP, "Local Plan Area", &drawingFunctorP); 



  // "Custom" commands. You can add your own custom commands here, they will
  // be available in MobileEyes' custom commands (enable in the toolbar or
  // access through Robot Tools)
  ArServerHandlerCommands commands(&server);


  // These provide various kinds of information to the client:
  ArServerInfoRobot serverInfoRobot(&server, &robot);
  ArServerInfoSensor serverInfoSensor(&server, &robot);
  ArServerInfoPath serverInfoPath(&server, &robot, &pathTask);
  serverInfoPath.addSearchRectangleDrawing(&drawings);
  serverInfoPath.addControlCommands(&commands);

#if SONAR_LOC
  // Provides localization info and allows the client (MobileEyes) to relocalize at a given
  // pose:
  ArServerInfoLocalization serverInfoLocalization(&server, &robot, &locTask);
  ArServerHandlerLocalization serverLocHandler(&server, &robot, &locTask);
#endif

  // If you're using MobileSim, ArServerHandlerLocalization sends it a command
  // to move the robot's true pose if you manually do a localization through 
  // MobileEyes.  To disable that behavior, use this constructor call instead:
  // ArServerHandlerLocalization serverLocHandler(&server, &robot, true, false);
  // The fifth argument determines whether to send the command to MobileSim.

  // Provide the map to the client (and related controls):
  ArServerHandlerMap serverMap(&server, &map);

  // These objects add some simple (custom) commands to 'commands' for testing and debugging:
  ArServerSimpleComUC uCCommands(&commands, &robot);                   // Send any command to the microcontroller
  ArServerSimpleComMovementLogging loggingCommands(&commands, &robot); // configure logging
  ArServerSimpleComLogRobotConfig configCommands(&commands, &robot);   // trigger logging of the robot config parameters
//  ArServerSimpleServerCommands serverCommands(&commands, &server);     // monitor networking behavior (track packets sent etc.)


  // service that allows the client to monitor the communication link status
  // between the robot and the client.
  //
  ArServerHandlerCommMonitor handlerCommMonitor(&server);



  // service that allows client to change configuration parameters in ArConfig 
  ArServerHandlerConfig handlerConfig(&server, Aria::getConfig(),
				      Arnl::getTypicalDefaultParamFileName(),
				      Aria::getDirectory());



  /* Set up the possible modes for remote control from a client such as
   * MobileEyes:
   */

  // Mode To go to a goal or other specific point:
  //FIXME: What are the arguments?! Documentation is missing constructor!
#if SONAR_LOC
  ArServerModeGoto modeGoto(&server, &robot, &pathTask, &map,
			    locTask.getRobotHome(),
			    locTask.getRobotHomeCallback());
#else
  ArServerModeGoto modeGoto(&server, &robot, &pathTask, &map);
#endif

  // Mode To stop and remain stopped:
  ArServerModeStop modeStop(&server, &robot);


  // Teleoperation modes To drive by keyboard, joystick, etc:
  ArServerModeRatioDrive modeRatioDrive(&server, &robot);  
//  ArServerModeDrive modeDrive(&server, &robot);            // Older mode for compatability



  // Prevent normal teleoperation driving if localization is lost using
  // a high-priority action, which enables itself when the particular mode is
  // active.
  // (You have to enter unsafe drive mode to drive when lost.)
#if SONAR_LOC
  ArActionLost actionLostRatioDrive(&locTask, &pathTask, &modeRatioDrive);
  modeRatioDrive.getActionGroup()->addAction(&actionLostRatioDrive, 110);
#endif

  // Add drive mode section to the configuration, and also some custom (simple) commands:
  modeRatioDrive.addToConfig(Aria::getConfig(), "Teleop settings");
  modeRatioDrive.addControlCommands(&commands);

  // Wander mode (also prevent wandering if lost):
  ArServerModeWander modeWander(&server, &robot);
#if SONAR_LOC
  ArActionLost actionLostWander(&locTask, &pathTask, &modeWander);
  modeWander.getActionGroup()->addAction(&actionLostWander, 110);
#endif


  // This provides a small table of interesting information for the client
  // to display to the operator. You can add your own callbacks to show any
  // data you want.
  ArServerInfoStrings stringInfo(&server);
  Aria::getInfoGroup()->addAddStringCallback(stringInfo.getAddStringFunctor());
  
  // Provide a set of informational data (turn on in MobileEyes with
  // View->Custom Details)

  Aria::getInfoGroup()->addStringInt(
	  "Motor Packet Count", 10, 
	  new ArConstRetFunctorC<int, ArRobot>(&robot, 
					       &ArRobot::getMotorPacCount));

#if SONAR_LOC
  Aria::getInfoGroup()->addStringDouble(
	  "Sonar Localization Score", 8, 
	  new ArRetFunctorC<double, ArSonarLocalizationTask>(
		  &locTask, 
      &ArSonarLocalizationTask::getLocalizationScore),
	  "%.03f");
  Aria::getInfoGroup()->addStringInt(
	  "Sonar Loc Num Samples", 8, 
	  new ArRetFunctorC<int, ArSonarLocalizationTask>(
		  &locTask, &ArSonarLocalizationTask::getCurrentNumSamples),
	  "%4d");
#endif


  // Display gyro status if gyro is enabled and is being handled by the firmware (gyro types 2, 3, or 4).
  // (If the firmware detects an error communicating with the gyro or IMU it
  // returns a flag, and stops using it.)
  // (This gyro type parameter, and fault flag, are only in ARCOS, not Seekur firmware)
  if(robot.getOrigRobotConfig() && robot.getOrigRobotConfig()->getGyroType() > 1)
  {
    Aria::getInfoGroup()->addStringString(
          "Gyro/IMU Status", 10,
          new ArGlobalRetFunctor1<const char*, ArRobot*>(&getGyroStatusString, &robot)
      );
  }





  // Make Stop mode the default (If current mode deactivates without entering
  // a new mode, then Stop Mode will be selected)
  modeStop.addAsDefaultMode();
    // TODO move up near where stop mode is created?





  /*
  // If we are on a simulator, move the robot back to its starting position,
  // and reset its odometry.
  // This will allow localizeRobotAtHomeBlocking() below will (probably) work (it
  // tries current odometry (which will be 0,0,0) and all the map
  // home points.
  // (Ignored by a real robot)
  //robot.com(ArCommands::SIM_RESET);
  */


  // create a pose storage class, this will let the program keep track
  // of where the robot is between runs...  after we try and restore
  // from this file it will start saving the robot's pose into the
  // file
  ArPoseStorage poseStorage(&robot);
  /// if we could restore the pose from then set the sim there (this
  /// won't do anything to the real robot)... if we couldn't restore
  /// the pose then just reset the position of the robot (which again
  /// won't do anything to the real robot)
#if SONAR_LOC
  if (poseStorage.restorePose("robotPose"))
    serverLocHandler.setSimPose(robot.getPose());
  else
    robot.com(ArCommands::SIM_RESET);
#endif

  /* File transfer services: */
  
#ifdef WIN32
  // Not implemented for Windows yet.
  ArLog::log(ArLog::Normal, "Note, file upload/download services are not implemented for Windows; not enabling them.");
#else
  // This block will allow you to set up where you get and put files
  // to/from, just comment them out if you don't want this to happen
  // /*
  ArServerFileLister fileLister(&server, fileDir);
  ArServerFileToClient fileToClient(&server, fileDir);
  ArServerFileFromClient fileFromClient(&server, fileDir, "/tmp");
  ArServerDeleteFileOnServer deleteFileOnServer(&server, fileDir);
  // */
#endif

    /* Video image streaming, and camera controls (Requires SAVserver or ACTS) */

  // Forward any video if either ACTS or SAV server are running.
  // You can find out more about SAV and ACTS on our website
  // http://robots.activmedia.com. ACTS is for color tracking and is
  // a seperate product. SAV just does software A/V transmitting and is
  // free to all our customers. Just run ACTS or SAV server before you
  // start this program and this class here will forward video from the
  // server to the client.
  ArHybridForwarderVideo videoForwarder(&server, "localhost", 7070);
  
  // make a camera to use in case we have video. the camera collection collects
  // multiple ptz cameras 
  ArPTZ *camera = NULL;
  ArServerHandlerCamera *handlerCamera = NULL;
  ArCameraCollection *cameraCollection = NULL;

  // if we have video then set up a camera 
  if (videoForwarder.isForwardingVideo())
  {

    cameraCollection = new ArCameraCollection();
    cameraCollection->addCamera("Cam1", "PTZ", "Camera", "PTZ");

    videoForwarder.setCameraName("Cam1");
    videoForwarder.addToCameraCollection(*cameraCollection);

    camera = new ArVCC4(&robot); //,	invertedCamera, ArVCC4::COMM_UNKNOWN, true, true);
    // To use an RVision SEE camera instead:
    // camera = new ArRVisionPTZ(&robot);
    camera->init();

    handlerCamera = new ArServerHandlerCamera("Cam1", 
		                                           &server, 
					                                     &robot,
					                                     camera, 
					                                     cameraCollection);

    pathTask.addGoalFinishedCB(
	    new ArFunctorC<ArServerHandlerCamera>(
		    handlerCamera, 
		    &ArServerHandlerCamera::cameraModeLookAtGoalClearGoal));
  }

  // After all of the cameras / videos have been created and added to the collection,
  // then start the collection server.
  //
  if (cameraCollection != NULL) {
    new ArServerHandlerCameraCollection(&server, cameraCollection);
  }


  /*********** Map update services ***********/

  ArMapChanger mapChanger(&server, &map);

  /******************************************/


    /* Load configuration values, map, and begin! */

  
  // When parsing the configuration file, also look at the program's command line options 
  // from the command-line argument parser as well as the configuration file.
  // (So you can use any argument on the command line, namely -map.) 
  Aria::getConfig()->useArgumentParser(&parser);

  // Read in parameter files.
  ArLog::log(ArLog::Normal, "Loading config file %s into ArConfig...", Arnl::getTypicalParamFileName());
  if (!Aria::getConfig()->parseFile(Arnl::getTypicalParamFileName()))
  {
    ArLog::log(ArLog::Normal, "Trouble loading configuration file, exiting");
    Aria::exit(5);
  }

  // Warn about unknown params.
  if (!simpleOpener.checkAndLog() || !parser.checkHelpAndWarnUnparsed())
  {
    logOptions(argv[0]);
    Aria::exit(6);
  }

  // Warn if there is no map
  if (map.getFileName() == NULL || strlen(map.getFileName()) <= 0)
  {
    ArLog::log(ArLog::Normal, "");
    ArLog::log(ArLog::Normal, "### No map file is set up, you can make a map with the following procedure");
    ArLog::log(ArLog::Normal, "   0) You can find this information in README.txt or docs/SonarMapping.txt");
    ArLog::log(ArLog::Normal, "   1) Start up Mapper3Basic");
    ArLog::log(ArLog::Normal, "   2) Go to File->New");
    ArLog::log(ArLog::Normal, "   3) Draw a line map of your area (make sure it is to scale)");
    ArLog::log(ArLog::Normal, "   4) Go to File->Save on Robot");
    ArLog::log(ArLog::Normal, "   5) In MobileEyes, go to Tools->Robot Config");
    ArLog::log(ArLog::Normal, "   6) Choose the Files section");
    ArLog::log(ArLog::Normal, "   7) Enter the path and name of your new .map file for the value of the Map parameter.");
    ArLog::log(ArLog::Normal, "   8) Press OK and your new map should become the map used");
    ArLog::log(ArLog::Normal, "");    
  }

  // Print a log message notifying user of the directory for map files
  ArLog::log(ArLog::Normal, "");
  ArLog::log(ArLog::Normal, 
	     "Directory for maps and file serving: %s", fileDir);
  
  ArLog::log(ArLog::Normal, "See the ARNL README.txt for more information");
  ArLog::log(ArLog::Normal, "");

  // Do an initial localization of the robot. It tries all the home points
  // in the map, as well as the robot's current odometric position, as possible
  // places the robot is likely to be at startup.   If successful, it will
  // also save the position it found to be the best localized position as the
  // "Home" position, which can be obtained from the localization task (and is
  // used by the "Go to home" network request).
#if SONAR_LOC
  locTask.localizeRobotAtHomeBlocking();
#endif

  // Start the networking server's thread
  server.runAsync();


  // Add a key handler so that you can exit by pressing
  // escape. Note that this key handler, however, prevents this program from
  // running in the background (e.g. as a system daemon or run from 
  // the shell with "&") -- it will lock up trying to read the keys; 
  // remove this if you wish to be able to run this program in the background.
  ArKeyHandler *keyHandler;
  if ((keyHandler = Aria::getKeyHandler()) == NULL)
  {
    keyHandler = new ArKeyHandler;
    Aria::setKeyHandler(keyHandler);
    robot.lock();
    robot.attachKeyHandler(keyHandler);
    robot.unlock();
    puts("Server running. To exit, press escape.");
  }

  // Enable the motors and wait until the robot exits (disconnection, etc.) or this program is
  // canceled.
  robot.enableMotors();
  robot.waitForRunExit();
  Aria::exit(0);
}

