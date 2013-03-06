#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QActionGroup>
#include <QComboBox>
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

protected:
    void closeEvent(QCloseEvent * event);

private slots:
    void connectToServer(QString host, int port, QString username, QString password);
    void on_actionConnect_triggered();
    void on_actionGoto_triggered();
    void on_actionStop_triggered();
    void on_actionResetToHome_triggered();
    void on_actionGotoHome_triggered();
    void on_actionExit_triggered();
    void on_actionAddObstacleRect_triggered(bool checked);
    void mapEditActionGroup_triggered(QAction *action);
    void on_actionDeleteMapObject_triggered(bool checked);
    void logData(ArRobotInfo pose, QPointF obstaclePos);
    void updateStatus(ArRobotInfo pose);
    void keyPressEvent(QKeyEvent * event);
    void untoggle();
    void lostConnection(QString reason);

    void on_actionShowMappedObstacles_triggered(bool checked);

private:
    Ui::MainWindow *ui;
    QActionGroup *_mapEditActionGroup;
    ConnectDialog *_connectDialog;
    MapScene *_mapScene;
    ArClient *_client;
    QFile *_dataFile;
    QString _user;
    QComboBox *_goalsComboBox;
};

#endif // MAINWINDOW_H
