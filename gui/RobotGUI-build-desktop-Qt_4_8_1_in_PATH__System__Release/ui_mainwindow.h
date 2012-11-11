/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Sun Nov 11 03:02:40 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGraphicsView>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QTextBrowser>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionConnect;
    QAction *actionQuit;
    QAction *actionRobotStop;
    QAction *actionRobotSendHome;
    QAction *actionMapAddWaypoint;
    QAction *actionAddObstacleLine;
    QWidget *centralWidget;
    QGroupBox *groupBoxGoals;
    QListWidget *listWidgetGoals;
    QGroupBox *groupBoxMap;
    QGraphicsView *graphicsViewMap;
    QGroupBox *groupBoxLog;
    QTextBrowser *textBrowserLog;
    QMenuBar *menuBar;
    QMenu *menuRobot;
    QToolBar *toolBarRobotControl;
    QStatusBar *statusBar;
    QToolBar *toolBarMapControl;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->setEnabled(true);
        MainWindow->resize(787, 646);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(787, 646));
        MainWindow->setMaximumSize(QSize(787, 646));
        actionConnect = new QAction(MainWindow);
        actionConnect->setObjectName(QString::fromUtf8("actionConnect"));
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionRobotStop = new QAction(MainWindow);
        actionRobotStop->setObjectName(QString::fromUtf8("actionRobotStop"));
        actionRobotStop->setCheckable(true);
        actionRobotSendHome = new QAction(MainWindow);
        actionRobotSendHome->setObjectName(QString::fromUtf8("actionRobotSendHome"));
        actionMapAddWaypoint = new QAction(MainWindow);
        actionMapAddWaypoint->setObjectName(QString::fromUtf8("actionMapAddWaypoint"));
        actionMapAddWaypoint->setCheckable(true);
        actionAddObstacleLine = new QAction(MainWindow);
        actionAddObstacleLine->setObjectName(QString::fromUtf8("actionAddObstacleLine"));
        actionAddObstacleLine->setCheckable(true);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        groupBoxGoals = new QGroupBox(centralWidget);
        groupBoxGoals->setObjectName(QString::fromUtf8("groupBoxGoals"));
        groupBoxGoals->setGeometry(QRect(10, 0, 161, 381));
        listWidgetGoals = new QListWidget(groupBoxGoals);
        listWidgetGoals->setObjectName(QString::fromUtf8("listWidgetGoals"));
        listWidgetGoals->setGeometry(QRect(10, 30, 141, 341));
        groupBoxMap = new QGroupBox(centralWidget);
        groupBoxMap->setObjectName(QString::fromUtf8("groupBoxMap"));
        groupBoxMap->setGeometry(QRect(171, 1, 601, 381));
        graphicsViewMap = new QGraphicsView(groupBoxMap);
        graphicsViewMap->setObjectName(QString::fromUtf8("graphicsViewMap"));
        graphicsViewMap->setGeometry(QRect(10, 30, 581, 341));
        groupBoxLog = new QGroupBox(centralWidget);
        groupBoxLog->setObjectName(QString::fromUtf8("groupBoxLog"));
        groupBoxLog->setGeometry(QRect(170, 390, 601, 141));
        textBrowserLog = new QTextBrowser(groupBoxLog);
        textBrowserLog->setObjectName(QString::fromUtf8("textBrowserLog"));
        textBrowserLog->setGeometry(QRect(10, 30, 581, 101));
        MainWindow->setCentralWidget(centralWidget);
        groupBoxGoals->raise();
        groupBoxGoals->raise();
        groupBoxMap->raise();
        groupBoxLog->raise();
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 787, 25));
        menuRobot = new QMenu(menuBar);
        menuRobot->setObjectName(QString::fromUtf8("menuRobot"));
        MainWindow->setMenuBar(menuBar);
        toolBarRobotControl = new QToolBar(MainWindow);
        toolBarRobotControl->setObjectName(QString::fromUtf8("toolBarRobotControl"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBarRobotControl);
        MainWindow->insertToolBarBreak(toolBarRobotControl);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        toolBarMapControl = new QToolBar(MainWindow);
        toolBarMapControl->setObjectName(QString::fromUtf8("toolBarMapControl"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBarMapControl);
        MainWindow->insertToolBarBreak(toolBarMapControl);

        menuBar->addAction(menuRobot->menuAction());
        menuRobot->addAction(actionConnect);
        menuRobot->addSeparator();
        menuRobot->addAction(actionQuit);
        toolBarRobotControl->addAction(actionRobotStop);
        toolBarMapControl->addAction(actionMapAddWaypoint);
        toolBarMapControl->addAction(actionAddObstacleLine);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionConnect->setText(QApplication::translate("MainWindow", "Connect ...", 0, QApplication::UnicodeUTF8));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", 0, QApplication::UnicodeUTF8));
        actionRobotStop->setText(QApplication::translate("MainWindow", "RobotStop", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionRobotStop->setToolTip(QApplication::translate("MainWindow", "Stop robot", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionRobotSendHome->setText(QApplication::translate("MainWindow", "RobotSendHome", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionRobotSendHome->setToolTip(QApplication::translate("MainWindow", "Send the robot home", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionMapAddWaypoint->setText(QApplication::translate("MainWindow", "MapAddWaypoint", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionMapAddWaypoint->setToolTip(QApplication::translate("MainWindow", "Add a waypoint", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionAddObstacleLine->setText(QApplication::translate("MainWindow", "AddObstacleLine", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionAddObstacleLine->setToolTip(QApplication::translate("MainWindow", "Add a linear obstacle", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        groupBoxGoals->setTitle(QApplication::translate("MainWindow", "Goals", 0, QApplication::UnicodeUTF8));
        groupBoxMap->setTitle(QApplication::translate("MainWindow", "Map", 0, QApplication::UnicodeUTF8));
        groupBoxLog->setTitle(QApplication::translate("MainWindow", "Log", 0, QApplication::UnicodeUTF8));
        menuRobot->setTitle(QApplication::translate("MainWindow", "Robot", 0, QApplication::UnicodeUTF8));
        toolBarMapControl->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
