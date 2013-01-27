#ifndef ROBOTGUI_H
#define ROBOTGUI_H

#include <vector>

#include <QMainWindow>
#include "connectdialog.h"
#include "robotclient.h"
#include "robotgraphic.h"
#include "robotmap.h"

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

    void on_buttonConnect_clicked();

private:
    Ui::robotGui *ui;
    ConnectDialog *_connectDialog;
    RobotMap *_robotMap;

    vector<RobotGraphic *> _robotGraphics;

    //FIXME: for now, just support one robot (ARIA)
    RobotClient *robot;
};

#endif // ROBOTGUI_H
