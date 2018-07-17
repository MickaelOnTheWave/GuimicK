#-------------------------------------------------
#
# Project created by QtCreator 2016-10-16T21:55:59
#
#-------------------------------------------------

QT       -= core gui

TARGET = ParsersLib
TEMPLATE = lib

contains( CONFIG, UseCurlLib ) {
   CONFIG += staticlib
}
else {
   CONFIG += shared_and_static build_all
}

DEFINES += PARSERSLIB_LIBRARY

SOURCES += \
    ../../src/aptgetupgradeparser.cpp \
    ../../src/clamavreportparser.cpp \
    ../../src/rsnapshotreportparser.cpp \
    ../../src/abstractoutputparser.cpp \
    ../../src/filebackupreport.cpp \
    ../../src/abstractfilebackupparser.cpp \
    ../../src/dfcommandparser.cpp \
    ../../src/rsynccommandparser.cpp \
    ../../src/tarcommandparser.cpp \
    ../../src/gitplumbingreportparser.cpp \
    ../../src/gitporcelainreportparser.cpp

HEADERS +=\
    ../../src/abstractoutputparser.h \
    ../../src/aptgetupgradeparser.h \
    ../../src/clamavreportparser.h \
    ../../src/rsnapshotreportparser.h \
    ../../src/filebackupreport.h \
    ../../src/abstractfilebackupparser.h \
    ../../src/dfcommandparser.h \
    ../../src/rsynccommandparser.h \
    ../../src/tarcommandparser.h \
    ../../src/gitplumbingreportparser.h \
    ../../src/gitporcelainreportparser.h

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
TOOLS_ROOT = /home/mickael/Prog/Tools
TOOLSLIB_ROOT = $$TOOLS_ROOT/ToolsLib
TOOLS_LIB_PATH = $$TOOLS_ROOT/bin/$$BUILD_TYPE/ToolsLib/proj/qmake
TOOLS_INCLUDE_PATH = $$TOOLSLIB_ROOT/src
LIBS += -L$$TOOLS_LIB_PATH -lToolsLib
INCLUDEPATH += $$TOOLS_INCLUDE_PATH
DEPENDPATH += $$TOOLS_INCLUDE_PATH


