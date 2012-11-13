/********************************************************************************
** Form generated from reading UI file 'robotgui.ui'
**
** Created: Tue Nov 13 17:28:01 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ROBOTGUI_H
#define UI_ROBOTGUI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGraphicsView>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_robotGui
{
public:
    QAction *actionConnect;
    QAction *action_Quit;
    QWidget *centralWidget;
    QTextEdit *textEdit;
    QGraphicsView *graphicsView;
    QMenuBar *menuBar;
    QMenu *menuRobot;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *robotGui)
    {
        if (robotGui->objectName().isEmpty())
            robotGui->setObjectName(QString::fromUtf8("robotGui"));
        robotGui->resize(352, 493);
        actionConnect = new QAction(robotGui);
        actionConnect->setObjectName(QString::fromUtf8("actionConnect"));
        action_Quit = new QAction(robotGui);
        action_Quit->setObjectName(QString::fromUtf8("action_Quit"));
        centralWidget = new QWidget(robotGui);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        textEdit = new QTextEdit(centralWidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(10, 310, 331, 111));
        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(10, 10, 331, 291));
        robotGui->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(robotGui);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 352, 25));
        menuRobot = new QMenu(menuBar);
        menuRobot->setObjectName(QString::fromUtf8("menuRobot"));
        robotGui->setMenuBar(menuBar);
        mainToolBar = new QToolBar(robotGui);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        robotGui->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(robotGui);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        robotGui->setStatusBar(statusBar);

        menuBar->addAction(menuRobot->menuAction());
        menuRobot->addAction(actionConnect);
        menuRobot->addSeparator();
        menuRobot->addAction(action_Quit);

        retranslateUi(robotGui);

        QMetaObject::connectSlotsByName(robotGui);
    } // setupUi

    void retranslateUi(QMainWindow *robotGui)
    {
        robotGui->setWindowTitle(QApplication::translate("robotGui", "robotGui", 0, QApplication::UnicodeUTF8));
        actionConnect->setText(QApplication::translate("robotGui", "&Connect ...", 0, QApplication::UnicodeUTF8));
        action_Quit->setText(QApplication::translate("robotGui", "&Quit", 0, QApplication::UnicodeUTF8));
        menuRobot->setTitle(QApplication::translate("robotGui", "Robot", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class robotGui: public Ui_robotGui {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ROBOTGUI_H
