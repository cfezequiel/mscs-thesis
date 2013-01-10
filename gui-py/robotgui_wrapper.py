#! /usr/bin/python

# UI wrapper containing UI behavior

import sys
from PyQt4 import QtCore, QtGui
from robotgui import Ui_robotGui

class RobotGUIWrapper(QtGui.QMainWindow):
    def __init__(self, parent=None):
        QtGui.QWidget.__init__(self, parent)
        self.ui = Ui_robotGui()
        self.ui.setupUi(self)

        # Connect action objects with corresponding functionalities
        self.ui.actionQuit.triggered.connect(QtGui.qApp.quit)
        self.ui.actionConnect.triggered.connect(self.openConnectDialog)

    def openConnectDialog(self):
        '''Open dialog box for inputting server connection information.'''

        pass


