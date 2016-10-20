#-------------------------------------------------
#
# Project created by QtCreator 2016-10-16T21:55:59
#
#-------------------------------------------------

QT       -= core gui

TARGET = ParsersLib
TEMPLATE = lib

DEFINES += PARSERSLIB_LIBRARY

SOURCES += \
    aptgetupgradeparser.cpp \
    clamavreportparser.cpp \
    gitreportparser.cpp \
    rsnapshotreportparser.cpp

HEADERS +=\
    abstractoutputparser.h \
    aptgetupgradeparser.h \
    clamavreportparser.h \
    gitreportparser.h \
    rsnapshotreportparser.h

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

