#ifndef ROBOTGUI_H
#define ROBOTGUI_H

#include <QMainWindow>

namespace Ui {
class robotGui;
}

class robotGui : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit robotGui(QWidget *parent = 0);
    ~robotGui();
    
private:
    Ui::robotGui *ui;
};

#endif // ROBOTGUI_H
