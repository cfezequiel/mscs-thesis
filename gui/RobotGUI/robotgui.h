#ifndef ROBOTGUI_H
#define ROBOTGUI_H

#include <QMainWindow>
#include "connectdialog.h"
#include "ariarobot.h"

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
    ConnectDialog *connectDialog;

    //FIXME: for now, just support one robot (ARIA)
    AriaRobot *robot;
};

#endif // ROBOTGUI_H
