#include <assert.h>
#include <iostream>
#include <sstream>

#include <QMessageBox>
#include <QTextStream>
#include <QDateTime>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "zone.h"
#include "mapdata.h"

using namespace std;

// Register metatype

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->mapView);

    // Set 'connect to server' dialog
    _connectDialog = new ConnectDialog(this);
    connect(_connectDialog, SIGNAL(connect(QString, int, QString, QString)),
            this, SLOT(connectToServer(QString, int, QString, QString)));

    // Set map scene
    _mapScene = new MapScene(this);
    ui->mapView->setScene(_mapScene);

    // Action group for map editing actions
    _mapEditActionGroup = new QActionGroup(this);
    ui->actionAddObstacleRect->setActionGroup(_mapEditActionGroup);
    ui->actionDeleteMapObject->setActionGroup(_mapEditActionGroup);
    QObject::connect(_mapEditActionGroup, SIGNAL(triggered(QAction *)),
                    this, SLOT(mapEditActionGroup_triggered(QAction *)));

    // Configure data logging
    _dataFile = new QFile("data.csv");
    QObject::connect(_mapScene, SIGNAL(sendData(ArRobotInfo, QPointF)),
                     this, SLOT(logData(ArRobotInfo, QPointF)));
    cout << _dataFile->fileName().toStdString() << endl;
    if (!_dataFile->exists())
    {
        if (_dataFile->open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream ts(_dataFile);
            ts << "Username,DateTime,RobotStatus,RobotMode,RobotXPos,RobotYpos,"
               << "RobotForwardVel,RobotRotationVel,ObstacleXPos,ObstacleYPos"
               << '\n'
                  ;
        }
        _dataFile->close();
    }
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

    // Check only the triggered action
    action->setChecked(true);
}

void MainWindow::connectToServer(QString host, int port, QString username, QString password)
{
    // Connect client
    QArClient *client = new QArClient;

    // Connect signals and slots
    // -- Client to MapScene --
    qRegisterMetaType<ArRobotInfo>("ArRobotInfo");
    QObject::connect((QObject *) client, SIGNAL(updateNumbers(ArRobotInfo)),
                     _mapScene, SLOT(updateRobotPose(ArRobotInfo)));
    QObject::connect((QObject *) client, SIGNAL(updateStrings(ArRobotInfo)),
                     _mapScene, SLOT(updateRobotPose(ArRobotInfo)));
    QObject::connect((QObject *) client, SIGNAL(updatePath(Points *)),
                     _mapScene, SLOT(updateRobotPath(Points *)));
    // -- MapScene to Client --
    QObject::connect((QObject *) _mapScene, SIGNAL(mapChanged(ArMap *)),
                     client, SLOT(mapChanged(ArMap *)));

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

    // Set as main client
    // FIXME: support only one robot for now
    _client = client;

    // Get map
    client->lock();
    ArMap *map = client->getMapFromServer();
    client->unlock();

    // Render map
    _mapScene->renderMap(map);
    //FIXME: correct the view dimensions
    ui->mapView->fitInView(_mapScene->sceneRect(), Qt::KeepAspectRatio);

    // Get periodic updates
    client->getUpdates(100);

    // Display connected status
    statusBar()->showMessage(QString("Connected."), 0);

    // Record user name
    //FIXME: convert this into a non-pointer object
    _user.append(username);

    // Enable toolbars
    ui->navToolBar->setEnabled(true);
    ui->mapEditToolBar->setEnabled(true);
}

void MainWindow::on_actionConnect_triggered()
{
    // TODO: Open dialog window
    _connectDialog->show();
    return;

}

void MainWindow::on_actionGoto_triggered()
{
    // FIXME: hardcoded goal value
    _client->goToGoal("G");

    // Request planned path
    _client->lock();
    _client->request("getPath", 1000);
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
