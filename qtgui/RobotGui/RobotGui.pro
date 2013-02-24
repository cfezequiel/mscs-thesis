#-------------------------------------------------
#
# Project created by QtCreator 2013-01-31T17:21:20
#
#-------------------------------------------------

QT       += core gui xml

TARGET = RobotGui
TEMPLATE = app


SOURCES += main.cpp\
           mainwindow.cpp\
           mapscene.cpp\
           ArClient.cpp \
    qarclient.cpp \
    robotobject.cpp \
    pathobject.cpp \
    forbiddenregion.cpp \
    connectdialog.cpp \
    mapitem.cpp \
    zone.cpp

HEADERS  += mainwindow.h\
            mapscene.h\
            ArClient.h \
    qarclient.h \
    robotobject.h \
    mapdata.h \
    pathobject.h \
    forbiddenregion.h \
    connectdialog.h \
    mapitem.h \
    zone.h

FORMS    += mainwindow.ui \
    connectdialog.ui

RESOURCES += \
    images.qrc

LIBS += -L/usr/local/Aria/lib -lAria -lArNetworking -lpthread -ldl -lrt

INCLUDEPATH += /usr/local/Aria/include \
               /usr/local/Aria/ArNetworking/include

