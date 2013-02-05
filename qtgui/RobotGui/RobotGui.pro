#-------------------------------------------------
#
# Project created by QtCreator 2013-01-31T17:21:20
#
#-------------------------------------------------

QT       += core gui

TARGET = RobotGui
TEMPLATE = app


SOURCES += main.cpp\
           mainwindow.cpp\
           mapscene.cpp\
           mapobject.cpp\
           ArClient.cpp \
    qarclient.cpp \
    robotobject.cpp \
    pathobject.cpp \
    obstacle.cpp

HEADERS  += mainwindow.h\
            mapscene.h\
            mapobject.h\
            ArClient.h \
    qarclient.h \
    robotobject.h \
    mapdata.h \
    pathobject.h \
    obstacle.h

FORMS    += mainwindow.ui

RESOURCES += \
    images.qrc

LIBS += -L/usr/local/Aria/lib -lAria -lArNetworking -lpthread -ldl -lrt

INCLUDEPATH += /usr/local/Aria/include \
               /usr/local/Aria/ArNetworking/include

