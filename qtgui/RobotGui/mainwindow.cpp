#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"


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
    QArClient *client = new QArClient();
    char host[] = "localhost";
    int port = 7272;

    if (!client->connect(host, port))
    {
        cerr << "Failed to connect to " << host;
        return;
    }

    // Set as main client
    _client = client;

    // Get map
    ArMap *map = client->getMap();

    // Render map
    _mapScene->renderMap(map);
    //FIXME: correct the view dimensions
    ui->mapView->fitInView(QRectF(-228,-8028, 1929, 10034),
                           Qt::KeepAspectRatio);

    // Get periodic updates
    client->getUpdates(100);

}
