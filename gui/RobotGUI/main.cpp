#include <QtGui/QApplication>
#include "robotgui.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    robotGui w;
    w.show();

    return a.exec();
}
