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
	 main.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

CONFIG(debug, debug|release) {
	BUILD_TYPE = Debug
}
else {
	BUILD_TYPE = Release
}

# linking ParsersTestLib
PARSERS_TEST_LIB_PATH = /home/mickael/Prog/TaskManager/bin/$$BUILD_TYPE/ReportParsers/ParsersTestLib/
PARSERS_TEST_INCLUDE_PATH = /home/mickael/Prog/TaskManager/ReportParsers/ParsersTestLib/
unix:!macx: LIBS += -L$$PARSERS_TEST_LIB_PATH -lParsersTestLib
INCLUDEPATH += $$PARSERS_TEST_INCLUDE_PATH
DEPENDPATH += $$PARSERS_TEST_INCLUDE_PATH

# linking ParsersLib
PARSERS_LIB_PATH = /home/mickael/Prog/TaskManager/bin/$$BUILD_TYPE/ReportParsers/ParsersLib/
PARSERS_INCLUDE_PATH = /home/mickael/Prog/TaskManager/ReportParsers/ParsersLib/
unix:!macx: LIBS += -L$$PARSERS_LIB_PATH -lParsersLib
INCLUDEPATH += $$PARSERS_INCLUDE_PATH
DEPENDPATH += $$PARSERS_INCLUDE_PATH

# linking ToolsLib
TOOLS_LIB_PATH = /home/mickael/Prog/Tools/bin/$$BUILD_TYPE/ToolsLib/
TOOLS_INCLUDE_PATH = /home/mickael/Prog/Tools/ToolsLib/
LIBS += -L$$TOOLS_LIB_PATH -lToolsLib

# linking QtToolsLib
QTTOOLS_LIB_PATH = /home/mickael/Prog/QtTools/bin/$$BUILD_TYPE/QtTools
QTTOOLS_INCLUDE_PATH = /home/mickael/Prog/QtTools/QtTools/
LIBS += -L$$QTTOOLS_LIB_PATH -lQtTools
INCLUDEPATH += $$QTTOOLS_INCLUDE_PATH
DEPENDPATH += $$QTTOOLS_INCLUDE_PATH

INCLUDEPATH += $$TOOLS_INCLUDE_PATH
DEPENDPATH += $$TOOLS_INCLUDE_PATH

HEADERS +=
