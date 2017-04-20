#-------------------------------------------------
#
# Project created by QtCreator 2016-10-16T21:55:59
#
#-------------------------------------------------

QT       -= core gui

TARGET = ParsersLib
TEMPLATE = lib

CONFIG += shared_and_static build_all

DEFINES += PARSERSLIB_LIBRARY

SOURCES += \
    aptgetupgradeparser.cpp \
    clamavreportparser.cpp \
    gitreportparser.cpp \
    rsnapshotreportparser.cpp \
    abstractoutputparser.cpp \
    filebackupreport.cpp \
    abstractfilebackupparser.cpp \
    dfcommandparser.cpp \
    gitcommitreportparser.cpp

HEADERS +=\
    abstractoutputparser.h \
    aptgetupgradeparser.h \
    clamavreportparser.h \
    gitreportparser.h \
    rsnapshotreportparser.h \
    filebackupreport.h \
    abstractfilebackupparser.h \
    dfcommandparser.h \
    gitcommitreportparser.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

contains( CONFIG, synology ) {
	BUILD_TYPE = Synology
}
else:CONFIG(debug, debug|release) {
	BUILD_TYPE = Debug
}
else {
	BUILD_TYPE = Release
}

# linking ToolsLib
TOOLS_LIB_PATH = /home/mickael/Prog/Tools/bin/$$BUILD_TYPE/ToolsLib/
TOOLS_INCLUDE_PATH = /home/mickael/Prog/Tools/ToolsLib/
LIBS += -L$$TOOLS_LIB_PATH -lToolsLib

INCLUDEPATH += $$TOOLS_INCLUDE_PATH
DEPENDPATH += $$TOOLS_INCLUDE_PATH

