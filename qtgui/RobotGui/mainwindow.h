#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;

    MapScene *_mapScene;
    ArClient *_client;
};

#endif // MAINWINDOW_H
