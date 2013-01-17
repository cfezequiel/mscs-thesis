#! /usr/bin/python

# UI wrapper containing UI behavior

import sys
from PyQt4 import QtCore, QtGui
from robotgui import Ui_robotGui
from connectdialog_wrapper import ConnectDialogWrapper

class RobotGUIWrapper(QtGui.QMainWindow):
    def __init__(self, parent=None):
        QtGui.QWidget.__init__(self, parent)
        self.ui = Ui_robotGui()
        self.ui.setupUi(self)
        self.connectDialog = ConnectDialogWrapper(self)
        self.robots = {}

        # Temporary
        self.client = 0

        # Connect objects with actions
        self.ui.actionQuit.triggered.connect(self.close)
        self.ui.actionConnect.triggered.connect(self.openConnectDialog)

    def openConnectDialog(self):
        '''Open dialog box for inputting server connection information.'''

        self.connectDialog.show()

    def closeEvent(self, event):
        print 'closeEvent'
        # FIXME: Causes flood of error messages
        #for robot in self.robots: 
        #   robot.disconnect()

        QtGui.qApp.quit()


