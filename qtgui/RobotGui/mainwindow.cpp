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
    // Connect client
    QDateTime dateTime = QDateTime::currentDateTime();
    QString session;
    QTextStream ts(&session);
    ts << username << '_' << dateTime.toString(QString("yyyy_MM_dd_hhmm"));
    QArClient *client = new QArClient(session);

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

    // Connect signal-slot for handling client disconnection
    QObject::connect(client, SIGNAL(disconnected(QString)),
                      this, SLOT(lostConnection(QString)));

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

    // Fill goals combobox with goal names
    QStringList goalList(_mapScene->goalList());
    _goalsComboBox->addItems(goalList);

    // Set map view
    ui->mapView->fitInView(_mapScene->sceneRect(), Qt::KeepAspectRatio);

    // Get periodic updates
    client->getUpdates(100);

    // Display connected status
    statusBar()->showMessage(QString("Connected."), 0);

    // Record user name
    _user.append(username);

    // Enable toolbars
    ui->navToolBar->setEnabled(true);
    ui->mapEditToolBar->setEnabled(true);

    // Enable graphics view
    ui->mapView->setEnabled(true);

    // Enable show mapped obstacles
    ui->actionShowMappedObstacles->setEnabled(true);

    // Configure data logging
    QString dataFilename;
    QTextStream ts2(&dataFilename);
    ts2 << client->getSessionName().c_str() << ".csv";
    _dataFile = new QFile(dataFilename);
    QObject::connect(_mapScene, SIGNAL(sendData(ArRobotInfo, QPointF)),
                     this, SLOT(logData(ArRobotInfo, QPointF)));
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

void MainWindow::on_actionConnect_triggered()
{
    // TODO: Open dialog window
    _connectDialog->show();
    return;

}

void MainWindow::on_actionGoto_triggered()
{
    // Go to goal whose name is selected in goals combo box
    QString goal = _goalsComboBox->currentText();
    _client->goToGoal(goal.toLocal8Bit().data());

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
    // Disconnect client
    if (_client != NULL)
    {
        if (_client->isConnected())
        {
            _client->stop();
            _client->disconnect();
        }
    }

    // Save map if it was loaded
    if (_mapScene->hasMap())
    {
        stringstream ss;
        string filename;
        ss << _client->getSessionName() << ".map";
        ss >> filename;
        _mapScene->getMap()->writeFile(filename.c_str());
    }
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

    // Clear map scene
    _mapScene->clear();

    // Disable toolbars
    ui->navToolBar->setEnabled(false);
    ui->mapEditToolBar->setEnabled(false);

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
