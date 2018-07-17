#-------------------------------------------------
#
# Project created by QtCreator 2016-10-16T21:57:17
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_reportparsers
CONFIG   += console
CONFIG   -= app_bundle
QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = app


SOURCES += \
	 ../../src/main.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

CONFIG(debug, debug|release) {
	BUILD_TYPE = Debug
}
else {
	BUILD_TYPE = Release
}

TASKMANAGER_ROOT = /home/mickael/Prog/TaskManager
PARSERS_ROOT = $$TASKMANAGER_ROOT/ReportParsers
PARSERS_BIN_ROOT = $$TASKMANAGER_ROOT/bin/$$BUILD_TYPE/ReportParsers

# linking ParsersTestLib
PARSERS_TEST_LIB_PATH = $$PARSERS_BIN_ROOT/ParsersTestLib/proj/qmake
PARSERS_TEST_INCLUDE_PATH = $$PARSERS_ROOT/ParsersTestLib/src/
unix:!macx: LIBS += -L$$PARSERS_TEST_LIB_PATH -lParsersTestLib
INCLUDEPATH += $$PARSERS_TEST_INCLUDE_PATH
DEPENDPATH += $$PARSERS_TEST_INCLUDE_PATH

# linking ParsersLib
PARSERS_LIB_PATH = $$PARSERS_BIN_ROOT/ParsersLib/proj/qmake
PARSERS_INCLUDE_PATH = $$PARSERS_ROOT/ParsersLib/src
unix:!macx: LIBS += -L$$PARSERS_LIB_PATH -lParsersLib
INCLUDEPATH += $$PARSERS_INCLUDE_PATH
DEPENDPATH += $$PARSERS_INCLUDE_PATH

# linking ToolsLib
TOOLS_ROOT = /home/mickael/Prog/Tools
TOOLSLIB_ROOT = $$TOOLS_ROOT/ToolsLib
TOOLS_LIB_PATH = $$TOOLS_ROOT/bin/$$BUILD_TYPE/ToolsLib/proj/qmake
TOOLS_INCLUDE_PATH = $$TOOLSLIB_ROOT/src
LIBS += -L$$TOOLS_LIB_PATH -lToolsLib
INCLUDEPATH += $$TOOLS_INCLUDE_PATH
DEPENDPATH += $$TOOLS_INCLUDE_PATH

# linking QtTools
QTTOOLSLIB_ROOT = /home/mickael/Prog/QtTools
QTTOOLS_LIB_PATH = $$QTTOOLSLIB_ROOT/bin/$$BUILD_TYPE/QtTools/proj/qmake
QTTOOLS_INCLUDE_PATH = $$QTTOOLSLIB_ROOT/QtTools/src
LIBS += -L$$QTTOOLS_LIB_PATH -lQtTools
INCLUDEPATH += $$QTTOOLS_INCLUDE_PATH
DEPENDPATH += $$QTTOOLS_INCLUDE_PATH

HEADERS +=
