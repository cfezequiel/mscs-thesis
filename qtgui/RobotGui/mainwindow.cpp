#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mapobject.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->mapView);

    // Set map scene
    _mapScene = new MapScene(this);
    ui->mapView->setScene(_mapScene);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionConnect_triggered()
{
    // TODO: Open dialog window

    // FIXME: temporary (refactor later)

    // Connect client
    QArClient *client = new QArClient;
    char host[] = "localhost";
    int port = 7272;

    // Connect signals and slots
    QObject::connect((QObject *) client, SIGNAL(updateNumbers(ArRobotInfo *)),
                     _mapScene, SLOT(updateRobotPose(ArRobotInfo *)));
    QObject::connect((QObject *) client, SIGNAL(updatePath(Points *)),
                     _mapScene, SLOT(updateRobotPath(Points *)));
    QObject::connect((QObject *) _mapScene, SIGNAL(addObstacle(Obstacle *)),
                     client, SLOT(addObstacle(Obstacle *)));

    if (!client->ArClient::connect(host, port))
    {
        cerr << "Failed to connect to " << host;
        // TODO: popup a message dialog box instead of printing
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
