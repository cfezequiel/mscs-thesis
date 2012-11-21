#include "robotgui.h"
#include "ui_robotgui.h"

robotGui::robotGui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::robotGui)
{
    ui->setupUi(this);
    _connectDialog = new ConnectDialog;
    _mapScene = new QGraphicsScene;
    _mapScene->setSceneRect(-300, -300, 600, 600);

    ui->mapGraphicsView->setScene(_mapScene);
    ui->mapGraphicsView->setDragMode(QGraphicsView::ScrollHandDrag);

}

robotGui::~robotGui()
{
    delete ui;
    delete _connectDialog;
    delete _mapScene;
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
    _connectDialog->show();
    if (_connectDialog->exec() != QDialog::Accepted)
    {
        return;
    }

    // Connect to robot
    // FIXME: for now, support only ARIA-based robot
    username = _connectDialog->getUsername();
    password = _connectDialog->getPassword();
    server = _connectDialog->getServer();
    robot = new AriaRobot;
    robot->connect(username.toStdString(),
                  password.toStdString(),
                  server.toStdString());

    // Show status on the log
    // TODO: create log widget
    QString msg;
    QTextStream msgStream(&msg, QIODevice::WriteOnly);
    msgStream << "Connected to robot server at " << server << endl;
    ui->logTextEdit->setText(msg);

    // Show robot on the map
    RobotGraphic *robotGraphic = new RobotGraphic;
    _robotGraphics.push_back(robotGraphic);
    _mapScene->addItem(robotGraphic);
}
