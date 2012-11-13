#include "robotgui.h"
#include "ui_robotgui.h"

robotGui::robotGui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::robotGui)
{
    ui->setupUi(this);
}

robotGui::~robotGui()
{
    delete ui;
}
