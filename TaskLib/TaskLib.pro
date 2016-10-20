#-------------------------------------------------
#
# Project created by QtCreator 2016-10-20T10:51:15
#
#-------------------------------------------------

QT       -= core gui

TARGET = TaskLib
TEMPLATE = lib

DEFINES += TASKLIB_LIBRARY

SOURCES += tasklib.cpp

HEADERS += tasklib.h\
        tasklib_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
