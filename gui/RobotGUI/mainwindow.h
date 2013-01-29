#ifndef ROBOTGUI_H
#define ROBOTGUI_H

#include <vector>

#include <QMainWindow>
#include "connectdialog.h"
#include "robotgraphic.h"
#include "ArClient.h"

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

    void on_buttonConnect_clicked();

private:
    Ui::MainWindow *ui;
    ConnectDialog *_connectDialog;

    vector<ArClient *> _clients;
};

#endif // ROBOTGUI_H
