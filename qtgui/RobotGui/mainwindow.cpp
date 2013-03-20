#include <assert.h>
#include <iostream>
#include <sstream>

#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QDateTime>

#include "forbiddenregion.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "zone.h"
#include "mapdata.h"

using namespace std;

// Register metatype

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _client(NULL),
    _dataFile(NULL),
    _user("")
{
    ui->setupUi(this);

    QWidget *mainWidget = new QWidget;
    mainWidget->setLayout(ui->mainLayout);
    setCentralWidget(mainWidget);

    _goalsComboBox = new QComboBox;
    ui->navToolBar->insertWidget(ui->actionGoto, _goalsComboBox);

    // Set 'connect to server' dialog
    _connectDialog = new ConnectDialog(this);
    connect(_connectDialog, SIGNAL(connect(QString, int, QString, QString)),
            this, SLOT(connectToServer(QString, int, QString, QString)));

    // Enable antialiasing
    ui->mapView->setRenderHints(QPainter::Antialiasing);

    // Set map scene
    _mapScene = new MapScene(this);
    ui->mapView->setScene(_mapScene);

    // Action group for map editing actions
    _mapEditActionGroup = new QActionGroup(this);
    _mapEditActionGroup->setExclusive(false);
    ui->actionAddObstacleRect->setActionGroup(_mapEditActionGroup);
    ui->actionDeleteMapObject->setActionGroup(_mapEditActionGroup);
    QObject::connect(_mapEditActionGroup, SIGNAL(triggered(QAction *)),
                    this, SLOT(mapEditActionGroup_triggered(QAction *)));

    // Connect untoggle signal for unchecking add obstacle rect action
    // after obstacle was placed successfully
    // FIXME: this is a hack
    QObject::connect(_mapScene, SIGNAL(untoggle()), this, SLOT(untoggle()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mapEditActionGroup_triggered(QAction *action)
{
    //FIXME: This is not an efficient solution, but I could not find
    // any other way to make the actions exclusive

    // Uncheck actions
    QList<QAction *> actions = _mapEditActionGroup->actions();
    for (QList<QAction *>::iterator i = actions.begin();
         i != actions.end(); i++)
    {
        if (*i != action)
        {
            (*i)->setChecked(false);
        }
    }
}

void MainWindow::connectToServer(QString host, int port, QString username, QString password)
{
    // Create client
    QArClient *client = new QArClient(username);

    // Connect client to server
    if (!client->connect(host, port, username, password))
    {
        QString msg;
        QTextStream ts(&msg);
        ts << "Error: Unable to connect to " << host;
        QMessageBox msgBox;
        msgBox.setText(msg);
        msgBox.exec();
        return;
    }

    // Connect signals and slots
    // -- Client to MapScene --
    qRegisterMetaType<ArRobotInfo>("ArRobotInfo");
    QObject::connect((QObject *) client, SIGNAL(updatePose(ArRobotInfo)),
                     _mapScene, SLOT(updateRobotPose(ArRobotInfo)));
    QObject::connect((QObject *) client, SIGNAL(updatePath(Points *)),
                     _mapScene, SLOT(updateRobotPath(Points *)));
    QObject::connect((QObject *) client, SIGNAL(updatePose(ArRobotInfo)),
                     this, SLOT(updateStatus(ArRobotInfo)));
    // -- MapScene to Client --
    QObject::connect((QObject *) _mapScene, SIGNAL(mapChanged(ArMap *)),
                     client, SLOT(mapChanged(ArMap *)));
    QObject::connect((QObject *) _mapScene, SIGNAL(stop()),
                     client, SLOT(stop()));

    // -- Client to main window --
    QObject::connect(client, SIGNAL(disconnected(QString)),
                      this, SLOT(lostConnection(QString)));

    // -- Teleop to client --
    QObject::connect((QObject *) &_teleop,
                     SIGNAL(sendInputs(double, double, double)),
                     client,
                     SLOT(ratioDrive(double, double, double)));

    // Set as main client
    // TODO: support multiple robots
    _client = client;

    // If no map exists
    if (_mapScene->getMap() == NULL)
    {
        // Get map from server
        client->lock();
        ArMap *map = client->getMapFromServer();
        client->unlock();
        _mapScene->renderMap(map);

        // Fill goals combobox with goal names
        QStringList goalList(_mapScene->goalList());
        _goalsComboBox->addItems(goalList);
    }
    else
    {
        // Send map to server
        _mapScene->updateMap();
    }

    // Set map view
    ui->mapView->fitInView(_mapScene->sceneRect(), Qt::KeepAspectRatio);

    // Get periodic updates
    client->getUpdates(100);

    // Display connected status
    statusBar()->showMessage(QString("Connected."), 0);

    // Record user name
    _user.append(username);

    // Enable map and nav controls
    toggleMapControls(true);
    toggleNavControls(true);

    // Start logging
    _sessionName.clear();
    QTextStream ts2(&_sessionName);
    ts2 << username << '_' << _mapScene->getMapName() << '_'
        << QDateTime::currentDateTime().toString(QString("yyyy_MM_dd_hh:mm:ss"));
    startDataLogging(_sessionName);
}

void MainWindow::disconnectFromServer()
{
    // Save map if it was loaded
    _mapScene->saveMap(_sessionName.append(".map"));

    // Disconnect client
    if (_client != NULL)
    {
        if (_client->isConnected())
        {
            _client->stop();
            _client->disconnect();
        }
#if 0
        // FIXME: this causes a segfault for some reason
        // However, this is necessary to prevent a memory leak
        // What to do?
        delete _client;
#endif
        _client = NULL;
    }

    // Clear map scene
    _mapScene->clear();

    // Clear goals combobox
    _goalsComboBox->clear();

    // Disable map/nav controls
    toggleMapControls(false);
    toggleNavControls(false);
}

void MainWindow::on_actionConnect_triggered()
{
    _connectDialog->show();
}

void MainWindow::on_actionGoto_triggered()
{
    // Go to goal whose name is selected in goals combo box
    QString goal = _goalsComboBox->currentText();
    _client->goToGoal(goal.toLocal8Bit().data());

    // Request planned path
    _client->lock();
    _client->request("getPath", 500);
    _client->unlock();
}

void MainWindow::on_actionStop_triggered()
{
    // Stop the robot
    _client->stop();
}

void MainWindow::on_actionResetToHome_triggered()
{
    // Sets the robot's current pose as home
    _client->lock();
    _client->requestOnce("resetToHome");
    _client->unlock();
}

void MainWindow::on_actionAddObstacleRect_triggered(bool checked)
{
    MapScene *scene = _mapScene;

    if (checked)
    {
        scene->setMode(MapScene::ModeAddObstacle);
    }
    else
    {
        scene->setMode(MapScene::ModeView);
    }
}

void MainWindow::on_actionDeleteMapObject_triggered(bool checked)
{
    MapScene *scene = _mapScene;

    if (checked)
    {
        scene->setMode(MapScene::ModeDelete);
    }
    else
    {
        scene->setMode(MapScene::ModeView);
    }
}

void MainWindow::logData(ArRobotInfo pose, QPointF obstaclePos)
{
    assert(_dataFile != NULL);
    assert(_user != NULL);

    // Log data to file
    if (_dataFile->open(QIODevice::Append| QIODevice::Text))
    {
        QDateTime dateTime = QDateTime::currentDateTime();
        QTextStream out(_dataFile);
        out << _user << ','
            << dateTime.toString() << ','
            << pose.status << ','
            << pose.mode << ','
            << pose.xpos << ','
            << pose.ypos << ','
            << pose.forwardVel << ','
            << pose.rotationVel << ','
            << obstaclePos.x() << ','
            << obstaclePos.y() << '\n';
    }
    _dataFile->close();
}

void MainWindow::on_actionGotoHome_triggered()
{
    // Go to home position at startup
    _client->goHome();

    // Request planned path
    _client->lock();
    _client->request("getPath", 1000);
    _client->unlock();
}

void MainWindow::updateStatus(ArRobotInfo pose)
{
    QString msg;
    QTextStream ts(&msg);
    ts << "x = " << pose.xpos << ", y = " << pose.ypos
       << "\t lvel (mm/s) = " << pose.forwardVel
       << "\t rvel (deg/s) = " << pose.rotationVel
       << "\t status: " << pose.status
       << "\t mode: " << pose.mode;

    ui->statusBar->showMessage(msg);
}

void MainWindow::on_actionExit_triggered()
{
    // Exit the program cleanly
    close();
}

void MainWindow::closeEvent(QCloseEvent * event)
{
   disconnectFromServer();
}

void MainWindow::keyPressEvent(QKeyEvent * event)
{
    // If spacebar pressed, stop the robot if it is moving
    if (event->key() == Qt::Key_Space)
    {
        if (_client != NULL && _client->isConnected())
        {
            _client->stop();
        }
        return;
    }

    // Check teleop
    if (ui->actionTeleop->isChecked())
    {
        switch (event->key())
        {
        case Qt::Key_W: case Qt::Key_Up: // forward
            _teleop.setTransRatio(100);
            break;
        case Qt::Key_S: case Qt::Key_Down: // reverse
            _teleop.setTransRatio(-100);
            break;
        case Qt::Key_A: case Qt::Key_Left: // left turn
            _teleop.setRotRatio(50);
            break;
        case Qt::Key_D: case Qt::Key_Right: // right turn
            _teleop.setRotRatio(-50);
            break;
        default:
            // do nothing
            break;
        }
    }
}

// Untoggle the "add obstacle" button
void MainWindow::untoggle()
{
    if (ui->actionAddObstacleRect->isChecked())
    {
        ui->actionAddObstacleRect->setChecked(false);
    }
}

void MainWindow::lostConnection(QString reason)
{
    // Show dialog box indicating client disconnect reason
    QMessageBox msgBox;
    QString msg;
    QTextStream ts(&msg);
    ts << "Lost connection to server: " << reason << endl;
    msgBox.setText(msg);
    msgBox.exec();

    // Disconnect client (officially)
    // This also clears the map
    disconnectFromServer();

    // Try to reconnect client
    // TODO
}

void MainWindow::on_actionShowMappedObstacles_triggered(bool checked)
{
    assert(_mapScene != NULL);

    ForbiddenRegion *mappedObstacle = _mapScene->getMappedObstacle();
    if (mappedObstacle != NULL)
    {
        if (checked)
        {
            // Add mapped obstacle to scene
            _mapScene->addItem(mappedObstacle);
        }
        else
        {
            // Removed mapped obstacle from scene
            _mapScene->removeItem(mappedObstacle);
        }
    }
}

void MainWindow::on_actionDisconnect_triggered()
{
    // Disconnect client
    disconnectFromServer();
}

void MainWindow::toggleNavControls(bool value)
{
    ui->navToolBar->setEnabled(value);
    ui->actionDisconnect->setEnabled(value);
    ui->actionConnect->setEnabled(!value);
}

void MainWindow::toggleMapControls(bool value)
{
    ui->mapEditToolBar->setEnabled(value);
    ui->mapView->setEnabled(value);
    ui->actionShowMappedObstacles->setEnabled(value);
}

void MainWindow::on_actionLoadMap_triggered()
{
    // Save current map if it exists
    _mapScene->saveMap(_sessionName.append(".map"));

    // Clear the old map scene
    _mapScene->clear();

    // Open file dialog box so user can select map file
    QString filename;
    filename = QFileDialog::getOpenFileName(this,
                                            tr("Load a map"), "./",
                                            tr("Map Files (*.map)"));
    if (filename == "")
    {
        return;
    }

    // Set session name
#if 0
    if (_user == "")
    {
        QString username = SessionDialog::getSessionName();
        _user = username;
    }
#else
    _user = SessionDialog::getSessionName();
#endif

    // Load the map from file
    _mapScene->loadMapFromFile(filename);

    // Fit map scene to the view
    ui->mapView->fitInView(_mapScene->sceneRect(), Qt::KeepAspectRatio);

    // Enable map controls
    toggleMapControls(true);

    // Erase and fill goals combobox with new goal names
    _goalsComboBox->clear();
    _goalsComboBox->addItems(_mapScene->goalList());

    // Start data logging
    _sessionName.clear();
    QTextStream ts(&_sessionName);
    ts << _user << '_' << _mapScene->getMapName() << '_' << QDateTime::currentDateTime().toString(QString("yyyy_MM_dd_hh:mm:ss"));
    startDataLogging(_sessionName);
}

void MainWindow::on_actionTeleop_triggered(bool checked)
{
    if (checked)
    {
        _teleop.setEnabled(true);
    }
    else
    {
        _teleop.setEnabled(false);
    }
}

void MainWindow::startDataLogging(QString sessionName)
{
    assert(_mapScene != NULL);

    QDateTime dateTime = QDateTime::currentDateTime();
    QString filename;
    QTextStream ts(&filename);
    ts << sessionName << ".csv";

    // Delete old datafile
    if (_dataFile != NULL)
    {
        delete _dataFile;
    }

    // Configure data logging
    _dataFile = new QFile(filename);
    QObject::connect(_mapScene, SIGNAL(sendData(ArRobotInfo, QPointF)),
                     this, SLOT(logData(ArRobotInfo, QPointF)));
    if (!_dataFile->exists())
    {
        if (_dataFile->open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream ts(_dataFile);
            ForbiddenRegion *fr = _mapScene->getMappedObstacle();
            if (fr != NULL)
            {
                QPointF center = fr->scenePos();
                ts << "MappedObstacle(x y)," << center.x() << ',' << center.y()
                   << '\n';
            }
            ts << "Username,DateTime,RobotStatus,RobotMode,RobotXPos,RobotYpos,"
               << "RobotForwardVel,RobotRotationVel,ObstacleXPos,ObstacleYPos"
               << '\n'
                  ;
        }
        _dataFile->close();
    }
}

