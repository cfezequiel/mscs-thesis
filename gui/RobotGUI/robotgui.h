#ifndef ROBOTGUI_H
#define ROBOTGUI_H

#include <vector>

#include <QMainWindow>
#include "connectdialog.h"
#include "robotgraphic.h"
#include "ArClient.h"

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

    vector<RobotGraphic *> _robotGraphics;
    vector<ArClient *> _clients;
};

#endif // ROBOTGUI_H
