# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'connectdialog.ui' #
# Created: Wed Jan 16 17:57:44 2013
#      by: PyQt4 UI code generator 4.9.1
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

class Ui_connectDialog(object):
    def setupUi(self, connectDialog):
        connectDialog.setObjectName(_fromUtf8("connectDialog"))
        connectDialog.resize(237, 223)
        self.formLayoutWidget = QtGui.QWidget(connectDialog)
        self.formLayoutWidget.setGeometry(QtCore.QRect(10, 10, 221, 151))
        self.formLayoutWidget.setObjectName(_fromUtf8("formLayoutWidget"))
        self.formLayout = QtGui.QFormLayout(self.formLayoutWidget)
        self.formLayout.setFieldGrowthPolicy(QtGui.QFormLayout.AllNonFixedFieldsGrow)
        self.formLayout.setMargin(0)
        self.formLayout.setObjectName(_fromUtf8("formLayout"))
        self.label = QtGui.QLabel(self.formLayoutWidget)
        self.label.setObjectName(_fromUtf8("label"))
        self.formLayout.setWidget(0, QtGui.QFormLayout.LabelRole, self.label)
        self.usernameLineEdit = QtGui.QLineEdit(self.formLayoutWidget)
        self.usernameLineEdit.setObjectName(_fromUtf8("usernameLineEdit"))
        self.formLayout.setWidget(0, QtGui.QFormLayout.FieldRole, self.usernameLineEdit)
        self.label_2 = QtGui.QLabel(self.formLayoutWidget)
        self.label_2.setObjectName(_fromUtf8("label_2"))
        self.formLayout.setWidget(1, QtGui.QFormLayout.LabelRole, self.label_2)
        self.passwordLineEdit = QtGui.QLineEdit(self.formLayoutWidget)
        self.passwordLineEdit.setObjectName(_fromUtf8("passwordLineEdit"))
        self.formLayout.setWidget(1, QtGui.QFormLayout.FieldRole, self.passwordLineEdit)
        self.label_3 = QtGui.QLabel(self.formLayoutWidget)
        self.label_3.setObjectName(_fromUtf8("label_3"))
        self.formLayout.setWidget(2, QtGui.QFormLayout.LabelRole, self.label_3)
        self.hostLineEdit = QtGui.QLineEdit(self.formLayoutWidget)
        self.hostLineEdit.setObjectName(_fromUtf8("hostLineEdit"))
        self.formLayout.setWidget(2, QtGui.QFormLayout.FieldRole, self.hostLineEdit)
        self.label_4 = QtGui.QLabel(self.formLayoutWidget)
        self.label_4.setObjectName(_fromUtf8("label_4"))
        self.formLayout.setWidget(3, QtGui.QFormLayout.LabelRole, self.label_4)
        self.portLineEdit = QtGui.QLineEdit(self.formLayoutWidget)
        self.portLineEdit.setObjectName(_fromUtf8("portLineEdit"))
        self.formLayout.setWidget(3, QtGui.QFormLayout.FieldRole, self.portLineEdit)
        self.horizontalLayoutWidget = QtGui.QWidget(connectDialog)
        self.horizontalLayoutWidget.setGeometry(QtCore.QRect(40, 170, 191, 31))
        self.horizontalLayoutWidget.setObjectName(_fromUtf8("horizontalLayoutWidget"))
        self.horizontalLayout = QtGui.QHBoxLayout(self.horizontalLayoutWidget)
        self.horizontalLayout.setMargin(0)
        self.horizontalLayout.setObjectName(_fromUtf8("horizontalLayout"))
        self.cancelButton = QtGui.QPushButton(self.horizontalLayoutWidget)
        self.cancelButton.setObjectName(_fromUtf8("cancelButton"))
        self.horizontalLayout.addWidget(self.cancelButton)
        self.connectButton = QtGui.QPushButton(self.horizontalLayoutWidget)
        self.connectButton.setObjectName(_fromUtf8("connectButton"))
        self.horizontalLayout.addWidget(self.connectButton)

        self.retranslateUi(connectDialog)
        QtCore.QObject.connect(self.cancelButton, QtCore.SIGNAL(_fromUtf8("clicked()")), connectDialog.reject)
        QtCore.QMetaObject.connectSlotsByName(connectDialog)

    def retranslateUi(self, connectDialog):
        connectDialog.setWindowTitle(QtGui.QApplication.translate("connectDialog", "Dialog", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("connectDialog", "Username:", None, QtGui.QApplication.UnicodeUTF8))
        self.label_2.setText(QtGui.QApplication.translate("connectDialog", "Password:", None, QtGui.QApplication.UnicodeUTF8))
        self.label_3.setText(QtGui.QApplication.translate("connectDialog", "Host:", None, QtGui.QApplication.UnicodeUTF8))
        self.label_4.setText(QtGui.QApplication.translate("connectDialog", "Port:", None, QtGui.QApplication.UnicodeUTF8))
        self.cancelButton.setText(QtGui.QApplication.translate("connectDialog", "Cancel", None, QtGui.QApplication.UnicodeUTF8))
        self.connectButton.setText(QtGui.QApplication.translate("connectDialog", "Connect", None, QtGui.QApplication.UnicodeUTF8))

