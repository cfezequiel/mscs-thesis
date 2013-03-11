#include <unistd.h>
#include <cassert>
#include <cstdio>
#include <sstream>
#include <fstream>
#include <iostream>
#include "ArClient.h"

#define DEBUG 1
#if DEBUG
#endif

using namespace std;

ArClient::ArClient() : ArClientBase()
{
    initialize();
    _sessionName = "tmp";
}

ArClient::ArClient(string sessionName): ArClientBase()
{
    initialize();
    _sessionName = sessionName;
}

ArClient::~ArClient()
{
    delete _configHandler;
    delete _clientFileFromClient;
    delete _clientFileLister;
    delete _getMapCB;
    delete _listCommandsCB;
    delete _updateNumbersCB;
    delete _updateStringsCB;
    delete _getPathCB;
}

void ArClient::initialize()
{
    // Initialize callbacks
    _getMapCB = new ArFunctor1C<ArClient, ArNetPacket *>(this,
                    &ArClient::_handleGetMap);
    _listCommandsCB = new ArFunctor1C<ArClient, ArNetPacket *>(this,
                      &ArClient::_handleListCommands);
    _updateNumbersCB = new ArFunctor1C<ArClient, ArNetPacket *>(this,
                      &ArClient::_handleUpdateNumbers);
    _updateStringsCB = new ArFunctor1C<ArClient, ArNetPacket *>(this,
                      &ArClient::_handleUpdateStrings);
    _getPathCB = new ArFunctor1C<ArClient, ArNetPacket *>(this,
                      &ArClient::_handleGetPath);
    _disconnectOnErrorCB = new ArFunctorC<ArClient>(this,
                            &ArClient::connectionError);
    _serverShutdownCB = new ArFunctorC<ArClient>(this,
                            &ArClient::serverShutdown);

    // Initialize housekeeping variables
    _mapReceived = false;
    _nCommands = 0;
    _commandsReceived = false;
    //_moving = false;
}

bool ArClient::connect(const char *host, int port, const char *username, const char *password)
{
    // Connect to server
    bool result = blockingConnect(host, port, true, username, password);
    if (!result)
    {
        cerr << "Error: Connection to " << host << " failed." << endl;
        return result;
    }

    // Add robot config handler
    _configHandler = new ArClientHandlerConfig(this);
    _configHandler->requestConfigFromServer();

    // Add file handlers
    _clientFileFromClient = new ArClientFileFromClient(this);
    _clientFileLister = new ArClientFileLister(this);

    // Add map handler
    addHandler("getMap", _getMapCB);

    // Add list commands handler
    addHandler("listCommands", _listCommandsCB);

    // Add update handlers
    addHandler("updateNumbers", _updateNumbersCB);
    addHandler("updateStrings", _updateStringsCB);

    // Add path handler
    addHandler("getPath", _getPathCB);

    // Add disconnect handler
    addDisconnectOnErrorCB(_disconnectOnErrorCB);
    addServerShutdownCB(_serverShutdownCB);

    // Run client
    runAsync();

    return true;
}

void ArClient::_handleListCommands(ArNetPacket *packet)
{
    _nCommands = (unsigned int) packet->bufToUByte2();

    char buf[STRLEN];
    for (unsigned int i = 0; i < _nCommands; i++)
    {
        packet->bufToStr(buf, STRLEN);
        string s(buf);
        _strListBuf.push_back(s);
    }

    _commandsReceived = true;
}

list<string> * ArClient::listCommands()
{
    _commandsReceived = false;
    _strListBuf.clear();

    if (!request("listCommands", 1000))
    {
        cerr << "Error: Request 'listCommands' failed" << endl;
        return NULL;
    }

    while (!_commandsReceived)
    {
        ArUtil::sleep(100);
    }

    return &_strListBuf;
}

// Get map from robot server
ArMap * ArClient::getMapFromServer()
{
    if (!request("getMap", -1))
    {
        cerr << "Error: Map request failed." << endl;
    }

    // Do some housekeeping
    _mapReceived = false;
    _strbuf.str("");

    while (!_mapReceived)
    {
        ArUtil::sleep(100);
    }

    // Save map strings to a file
    stringstream ss;
    ss << _sessionName << ".map";
    char filename[256];
    ss >> filename;
    ofstream outFile(filename, ios::out);
    outFile << _strbuf.rdbuf();
    outFile.close();

    // Load map
    ArMap *map = new ArMap();
    if (!map->readFile(filename))
    {
        cerr << "Error: Unable to load map file." << endl;
        delete map;
        return NULL;
    }

    // Remove temporary map file
    if (remove(filename) != 0)
    {
        cerr << "Error: Unable to remove temporary map file." << endl;
    }

    return map;
}

void ArClient::_handleGetMap(ArNetPacket *packet)
{
    if (_mapReceived)
    {
        return;
    }

    char data[1024];
    int length = packet->getDataLength();
    packet->bufToData(data, length);

#if 0
    cout << "RCVD: " << data << endl;
#endif

    _strbuf << data << '\n';

    // All map data received
    if (data[0] == '\0')
    {
        _mapReceived = true;
    }
}

ArConfig * ArClient::getConfig()
{
    _configHandler->requestConfigFromServer();
    ArUtil::sleep(100);

    return _configHandler->getConfig();
}

void ArClient::sendMap(ArMap *map)
{
    assert(map != NULL);

    // Store last mode (for resuming later after config update)
    _lastMode = _robotInfo.mode;

    // Write map to file
    char *baseDir = get_current_dir_name();
    map->setBaseDirectory(baseDir);
    map->writeFile(map->getFileName());
    delete baseDir;

    // Get full path of map file in local directory
    stringstream ss;
    char mapFile[STRLEN];
    ss << map->getBaseDirectory() << '/' << map->getFileName();
    ss >> mapFile;

    // Send map file to robot server
    lock();
    // FIXME: hardcoded directory string
    char serverDir[STRLEN] = "tmp";
    _clientFileFromClient->putFileToDirectory(serverDir, map->getFileName(),
                                              mapFile);
    unlock();

    // Update location of map file in config
    // NOTE: This automatically reloads the configuration
    stringstream ss2;
    ss2 << serverDir << '/' << map->getFileName();
    ss2 >> mapFile;
    lock();
    setMapFileConfigOnServer(mapFile);
    unlock();

    // FIXME: not needed?
    // Try reloading the configuration manually
    _configHandler->reloadConfigOnServer();

    // Give some time for robot to path plan (FIXME: needed?)
    ArUtil::sleep(100);

    // Resume robot from previous action
    resume();
}

void ArClient::getUpdates(int frequency)
{
    request("updateNumbers", frequency);
    request("updateStrings", -1);
}

void ArClient::_handleUpdateNumbers(ArNetPacket *packet)
{
    lock();
    _robotInfo.batVoltage = packet->bufToByte2();
    _robotInfo.xpos = packet->bufToByte4();
    _robotInfo.ypos = packet->bufToByte4();
    _robotInfo.theta = packet->bufToByte2();
    _robotInfo.forwardVel = packet->bufToByte2();
    _robotInfo.rotationVel = packet->bufToByte2();
    updateNumbersReceived(_robotInfo);
    unlock();
}

void ArClient::_handleUpdateStrings(ArNetPacket *packet)
{
    lock();
    packet->bufToStr(_robotInfo.status, STRLEN);
    packet->bufToStr(_robotInfo.mode, STRLEN);
    updateStringsReceived(_robotInfo);
    unlock();
}

void ArClient::_handleGetPath(ArNetPacket *packet)
{
    int numPoints = packet->bufToByte2();
    _path.data.clear();

    Point point;
    for (int i = 0; i < numPoints; i++)
    {
        point.x = packet->bufToByte4();
        point.y = packet->bufToByte4();
        _path.data.push_back(point);
    }

    getPathReceived(&_path);
}

void ArClient::setMapFileConfigOnServer(const char *filename)
{
    assert(filename != NULL);

#if 0
    cout << "Map filename to send to server: " << filename << endl;
#endif

    // Get configuration
    ArConfig *config = _configHandler->getConfig();
    ArConfigSection *section = config->findSection("Files");

    // Debug
    if (section)
    {
        ArConfigArg *arg = section->findParam("Map");
        if (arg)
        {
            // Modify configuration
            bool result = arg->setString(filename);
            if (!result)
            {
                printf("Error updating server config file with new map.\n");
            }
        }
    }
    else
    {
        cerr << "Error: Could not find Files section." << endl;
    }

    // Save configuration on server
    _configHandler->saveConfigToServer();
}

void ArClient::stop()
{
    lock();
    requestOnce("stop");
    unlock();
}

void ArClient::goToGoal(const char *goalName)
{
    assert(goalName != NULL);

    lock();
    requestOnceWithString("gotoGoal", goalName);
    unlock();

    _currentGoal = goalName;
    //_moving = true;
}

void ArClient::goHome()
{
    lock();
    requestOnce("home");
    unlock();
}

void ArClient::resume()
{
    if (_lastMode == "Goto goal" &&
            string(_robotInfo.status).find("Going to") == 0)
    {
        goToGoal(_currentGoal.c_str());
    }
    else if (_lastMode == "Go home")
    {
        goHome();
    }
    else
    {
        // do nothing (stopped?)
    }
}

string ArClient::getSessionName()
{
    return _sessionName;
}
