TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += static

SOURCES += ../../src/main.cpp

contains( CONFIG, synology ) {
	BUILD_TYPE = Synology
}
else:CONFIG(debug, debug|release) {
	BUILD_TYPE = Debug
}
else {
	BUILD_TYPE = Release
}

# linking ParsersLib
TASKMANAGER_ROOT = /home/mickael/Prog/TaskManager
PARSERS_LIB_PATH = $$TASKMANAGER_ROOT/bin/$$BUILD_TYPE/ReportParsers/ParsersLib/proj/qmake
PARSERS_INCLUDE_PATH = $$TASKMANAGER_ROOT/ReportParsers/ParsersLib/src
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
