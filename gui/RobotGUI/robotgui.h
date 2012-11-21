#ifndef ROBOTGUI_H
#define ROBOTGUI_H

#include <vector>

#include <QMainWindow>
#include "connectdialog.h"
#include "ariarobot.h"
#include "robotgraphic.h"

namespace Ui {
class robotGui;
}

class robotGui : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit robotGui(QWidget *parent = 0);
    ~robotGui();
    
private slots:
    void on_actionConnect_triggered();

private:
    Ui::robotGui *ui;
    ConnectDialog *_connectDialog;
    QGraphicsScene *_mapScene;

    vector<RobotGraphic *> _robotGraphics;

    //FIXME: for now, just support one robot (ARIA)
    AriaRobot *robot;
};

#endif // ROBOTGUI_H
