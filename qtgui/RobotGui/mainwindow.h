#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QActionGroup>
#include <QMainWindow>
#include <QObject>

#include "mapscene.h"
#include "qarclient.h"

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
    void on_actionConnect_triggered();
    void on_actionGoto_triggered();

    void on_actionStop_triggered();

    void on_actionResetToHome_triggered();

    void on_actionAddObstacleRect_triggered(bool checked);
    void on__mapEditActionGroup_triggered(QAction *action);

    void on_actionDeleteMapObject_triggered(bool checked);

private:
    Ui::MainWindow *ui;
    QActionGroup *_mapEditActionGroup;

    MapScene *_mapScene;
    ArClient *_client;
};

#endif // MAINWINDOW_H
