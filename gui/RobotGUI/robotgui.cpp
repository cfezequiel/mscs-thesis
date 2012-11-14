#include "robotgui.h"
#include "ui_robotgui.h"

robotGui::robotGui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::robotGui)
{
    ui->setupUi(this);
    connectDialog = new ConnectDialog;
}

robotGui::~robotGui()
{
    delete ui;
}

void robotGui::on_actionConnect_triggered()
{
    QString username;
    QString password;
    QString server;

    // Open dialog box to ask for the following information:
    // IP Address (including port)
    // Username
    // Password
    connectDialog->show();
    if (connectDialog->exec() != QDialog::Accepted)
    {
        return;
    }

    // Connect to robot
    // FIXME: for now, support only ARIA-based robot
    username = connectDialog->getUsername();
    password = connectDialog->getPassword();
    server = connectDialog->getServer();
    robot = new AriaRobot;
    robot->connect(username.toStdString(),
                  password.toStdString(),
                  server.toStdString());

    // Show status on the log

    // Show robot on the map
}
