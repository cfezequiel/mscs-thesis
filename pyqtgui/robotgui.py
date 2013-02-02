# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'robotgui.ui'
#
# Created: Sun Jan 20 00:00:01 2013
#      by: PyQt4 UI code generator 4.9.1
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

class Ui_robotGui(object):
    def setupUi(self, robotGui):
        robotGui.setObjectName(_fromUtf8("robotGui"))
        robotGui.resize(443, 749)
        self.centralWidget = QtGui.QWidget(robotGui)
        self.centralWidget.setObjectName(_fromUtf8("centralWidget"))
        self.mapView = QtGui.QGraphicsView(self.centralWidget)
        self.mapView.setGeometry(QtCore.QRect(10, 10, 421, 431))
        self.mapView.setObjectName(_fromUtf8("mapView"))
        self.log = QtGui.QTextBrowser(self.centralWidget)
        self.log.setGeometry(QtCore.QRect(10, 450, 421, 161))
        self.log.setObjectName(_fromUtf8("log"))
        self.buttonUpdate = QtGui.QPushButton(self.centralWidget)
        self.buttonUpdate.setGeometry(QtCore.QRect(160, 630, 98, 27))
        self.buttonUpdate.setObjectName(_fromUtf8("buttonUpdate"))
        robotGui.setCentralWidget(self.centralWidget)
        self.menuBar = QtGui.QMenuBar(robotGui)
        self.menuBar.setGeometry(QtCore.QRect(0, 0, 443, 25))
        self.menuBar.setObjectName(_fromUtf8("menuBar"))
        self.menuRobot = QtGui.QMenu(self.menuBar)
        self.menuRobot.setObjectName(_fromUtf8("menuRobot"))
        robotGui.setMenuBar(self.menuBar)
        self.navToolbar = QtGui.QToolBar(robotGui)
        self.navToolbar.setObjectName(_fromUtf8("navToolbar"))
        robotGui.addToolBar(QtCore.Qt.TopToolBarArea, self.navToolbar)
        self.statusBar = QtGui.QStatusBar(robotGui)
        self.statusBar.setObjectName(_fromUtf8("statusBar"))
        robotGui.setStatusBar(self.statusBar)
        self.mapToolbar = QtGui.QToolBar(robotGui)
        self.mapToolbar.setObjectName(_fromUtf8("mapToolbar"))
        robotGui.addToolBar(QtCore.Qt.TopToolBarArea, self.mapToolbar)
        self.toolBar = QtGui.QToolBar(robotGui)
        self.toolBar.setObjectName(_fromUtf8("toolBar"))
        robotGui.addToolBar(QtCore.Qt.TopToolBarArea, self.toolBar)
        self.actionConnect = QtGui.QAction(robotGui)
        self.actionConnect.setObjectName(_fromUtf8("actionConnect"))
        self.actionQuit = QtGui.QAction(robotGui)
        self.actionQuit.setObjectName(_fromUtf8("actionQuit"))
        self.actionStop = QtGui.QAction(robotGui)
        self.actionStop.setEnabled(True)
        self.actionStop.setVisible(True)
        self.actionStop.setObjectName(_fromUtf8("actionStop"))
        self.actionGoto = QtGui.QAction(robotGui)
        self.actionGoto.setObjectName(_fromUtf8("actionGoto"))
        self.actionAddGoal = QtGui.QAction(robotGui)
        self.actionAddGoal.setObjectName(_fromUtf8("actionAddGoal"))
        self.menuRobot.addAction(self.actionConnect)
        self.menuRobot.addSeparator()
        self.menuRobot.addAction(self.actionQuit)
        self.menuBar.addAction(self.menuRobot.menuAction())
        self.navToolbar.addAction(self.actionStop)
        self.navToolbar.addAction(self.actionGoto)

        self.retranslateUi(robotGui)
        QtCore.QMetaObject.connectSlotsByName(robotGui)

    def retranslateUi(self, robotGui):
        robotGui.setWindowTitle(QtGui.QApplication.translate("robotGui", "robotGui", None, QtGui.QApplication.UnicodeUTF8))
        self.buttonUpdate.setText(QtGui.QApplication.translate("robotGui", "Update", None, QtGui.QApplication.UnicodeUTF8))
        self.menuRobot.setTitle(QtGui.QApplication.translate("robotGui", "Robot", None, QtGui.QApplication.UnicodeUTF8))
        self.mapToolbar.setWindowTitle(QtGui.QApplication.translate("robotGui", "toolBar", None, QtGui.QApplication.UnicodeUTF8))
        self.toolBar.setWindowTitle(QtGui.QApplication.translate("robotGui", "toolBar", None, QtGui.QApplication.UnicodeUTF8))
        self.actionConnect.setText(QtGui.QApplication.translate("robotGui", "&Connect ...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionConnect.setToolTip(QtGui.QApplication.translate("robotGui", "Connect to a robot", None, QtGui.QApplication.UnicodeUTF8))
        self.actionQuit.setText(QtGui.QApplication.translate("robotGui", "&Quit", None, QtGui.QApplication.UnicodeUTF8))
        self.actionStop.setText(QtGui.QApplication.translate("robotGui", "Stop", None, QtGui.QApplication.UnicodeUTF8))
        self.actionStop.setToolTip(QtGui.QApplication.translate("robotGui", "Stop the robot", None, QtGui.QApplication.UnicodeUTF8))
        self.actionGoto.setText(QtGui.QApplication.translate("robotGui", "Goto", None, QtGui.QApplication.UnicodeUTF8))
        self.actionGoto.setToolTip(QtGui.QApplication.translate("robotGui", "Go to next goal", None, QtGui.QApplication.UnicodeUTF8))
        self.actionAddGoal.setText(QtGui.QApplication.translate("robotGui", "AddGoal", None, QtGui.QApplication.UnicodeUTF8))
        self.actionAddGoal.setToolTip(QtGui.QApplication.translate("robotGui", "Add a goal", None, QtGui.QApplication.UnicodeUTF8))

