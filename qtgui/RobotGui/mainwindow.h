#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QActionGroup>
#include <QMainWindow>
#include <QObject>
#include <QFile>

#include "mapscene.h"
#include "qarclient.h"
#include "connectdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void connectToServer(QString host, int port, QString username, QString password);
    void on_actionConnect_triggered();
    void on_actionGoto_triggered();
    void on_actionStop_triggered();
    void on_actionResetToHome_triggered();
    void on_actionAddObstacleRect_triggered(bool checked);
    void mapEditActionGroup_triggered(QAction *action);
    void on_actionDeleteMapObject_triggered(bool checked);
    void logData(ArRobotInfo pose, QPointF obstaclePos);

private:
    Ui::MainWindow *ui;
    QActionGroup *_mapEditActionGroup;
    ConnectDialog *_connectDialog;
    MapScene *_mapScene;
    ArClient *_client;
    QFile *_dataFile;
    QString _user;
};

#endif // MAINWINDOW_H
