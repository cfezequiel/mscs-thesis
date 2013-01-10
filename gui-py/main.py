#! /usr/bin/python

import sys
from PyQt4 import QtGui
from robotgui_wrapper import RobotGUIWrapper

if __name__ == '__main__':
    app = QtGui.QApplication(sys.argv)
    ui = RobotGUIWrapper()
    ui.show()
    sys.exit(app.exec_())


