#include <iostream>
#include <vector>

#include "mainwindow.h"
#include "ui_robotgui.h"

using namespace std;

// Constructor
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _connectDialog = new ConnectDialog;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _connectDialog;

    for (vector<ArClient *>::iterator i = _clients.begin();
         i != _clients.end(); i++)
    {
        (*i)->disconnect();
        delete (*i);
    }
}

void MainWindow::on_actionConnect_triggered()
{
#if 0
    QString username;
    QString password;
    QString server;

    // Open dialog box to ask for the following information:
    // IP Address (including port)
    // Username
    // Password
    _connectDialog->show();
    if (_connectDialog->exec() != QDialog::Accepted)
    {
        return;
    }

    // Connect to robot
    username = _connectDialog->getUsername();
    password = _connectDialog->getPassword();
    server = _connectDialog->getServer();
    robot = new RobotClient;
    robot->connect(username.toStdString(),
                  password.toStdString(),
                  server.toStdString());

    // Show status on the log
    //TODO: create log widget
    QString msg;
    QTextStream msgStream(&msg, QIODevice::WriteOnly);
    msgStream << "Connected to robot server at " << server << endl;
    ui->logTextEdit->setText(msg);

    // Show robot on the map interface
    RobotGraphic *robotGraphic = new RobotGraphic;
    _robotGraphics.push_back(robotGraphic);
    _robotMap->addItem(robotGraphic);
#endif

}

void MainWindow::on_buttonConnect_clicked()
{
    // Create client
    ArClient *client = new ArClient();

    // Connect to server
    char host[] = "localhost";
    int port = 7272;
    char username[] = "";
    char password[] = "";
    if (!client->connect(host, port, username, password))
    {
        cerr << "Failed to connect to " << host;
        return;
    }

    // Add client to collection
    _clients.push_back(client);

    // Get map
    // FIXME: This is temporary. It should be that the client sends the map
    // To the server.
    ArMap *map = client->getMap();
    if (map == NULL)
    {
        cerr << "Error: Unable to retrieve map." << endl;
    }

    // Display map

    // Get robot info

}
