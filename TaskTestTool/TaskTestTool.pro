TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp

CONFIG(debug, debug|release) {
	BUILD_TYPE = Debug
}
else {
	BUILD_TYPE = Release
}

# linking TaskLib
TASK_LIB_PATH = /home/mickael/Prog/TaskManager/bin/$$BUILD_TYPE/TaskLib/
TASK_INCLUDE_PATH = /home/mickael/Prog/TaskManager/TaskLib/
LIBS += -L$$TASK_LIB_PATH -lTaskLib
INCLUDEPATH += $$TASK_INCLUDE_PATH
DEPENDPATH += $$TASK_INCLUDE_PATH

# linking ParsersLib
PARSERS_LIB_PATH = /home/mickael/Prog/TaskManager/bin/$$BUILD_TYPE/ReportParsers/ParsersLib/
PARSERS_INCLUDE_PATH = /home/mickael/Prog/TaskManager/ReportParsers/ParsersLib/
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

# linking QtToolsLib
QTTOOLS_LIB_PATH = /home/mickael/Prog/QtTools/bin/$$BUILD_TYPE/QtTools
QTTOOLS_INCLUDE_PATH = /home/mickael/Prog/QtTools/QtTools/
LIBS += -L$$QTTOOLS_LIB_PATH -lQtTools
INCLUDEPATH += $$QTTOOLS_INCLUDE_PATH
DEPENDPATH += $$QTTOOLS_INCLUDE_PATH

# linking TaskTestLib
TASKTEST_LIB_PATH = /home/mickael/Prog/TaskManager/bin/$$BUILD_TYPE/TaskTestLib
TASKTEST_INCLUDE_PATH = /home/mickael/Prog/TaskManager/TaskTestLib/
LIBS += -L$$TASKTEST_LIB_PATH -lTaskTestLib
INCLUDEPATH += $$TASKTEST_INCLUDE_PATH
DEPENDPATH += $$TASKTEST_INCLUDE_PATH

# linking ParsersTestLib
PARSERS_TEST_LIB_PATH = /home/mickael/Prog/TaskManager/bin/$$BUILD_TYPE/ReportParsers/ParsersTestLib/
PARSERS_TEST_INCLUDE_PATH = /home/mickael/Prog/TaskManager/ReportParsers/ParsersTestLib/
unix:!macx: LIBS += -L$$PARSERS_TEST_LIB_PATH -lParsersTestLib
INCLUDEPATH += $$PARSERS_TEST_INCLUDE_PATH
DEPENDPATH += $$PARSERS_TEST_INCLUDE_PATH

# linking QtToolsTestLib
QTTOOLS_TEST_LIB_PATH = /home/mickael/Prog/QtTools/bin/$$BUILD_TYPE/QtToolsTestLib/
QTTOOLS_TEST_INCLUDE_PATH = /home/mickael/Prog/QtTools/QtToolsTestLib/
unix:!macx: LIBS += -L$$QTTOOLS_TEST_LIB_PATH -lQtToolsTestLib
INCLUDEPATH += $$QTTOOLS_TEST_INCLUDE_PATH
DEPENDPATH += $$QTTOOLS_TEST_INCLUDE_PATH

# linking ToolsTestLib
TOOLS_TEST_LIB_PATH = /home/mickael/Prog/Tools/bin/$$BUILD_TYPE/ToolsTestLib
TOOLS_TEST_INCLUDE_PATH = /home/mickael/Prog/Tools/ToolsTestLib/
LIBS += -L$$TOOLS_TEST_LIB_PATH -lToolsTestLib
INCLUDEPATH += $$TOOLS_TEST_INCLUDE_PATH
DEPENDPATH += $$TOOLS_TEST_INCLUDE_PATH
