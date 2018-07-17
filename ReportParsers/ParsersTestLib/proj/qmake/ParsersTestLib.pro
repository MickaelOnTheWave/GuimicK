#-------------------------------------------------
#
# Project created by QtCreator 2017-03-20T18:50:45
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = ParsersTestLib
TEMPLATE = lib
#CONFIG += staticlib
QMAKE_CXXFLAGS += -std=c++11

SOURCES += ../../src/parserstestlib.cpp \
    ../../src/rsnapshotreportparsertest.cpp \
    ../../src/abstractoutputparsertest.cpp \
    ../../src/aptupgradeparsertest.cpp \
    ../../src/dfcommandparsertest.cpp \
    ../../src/filebackupparserabstracttest.cpp \
    ../../src/mockparser.cpp \
    ../../src/rsynccommandparsertest.cpp \
    ../../src/tarcommandparsertest.cpp \
    ../../src/gitplumbingreportparsertest.cpp \
    ../../src/gitporcelainreportparsertest.cpp

HEADERS += ../../src/parserstestlib.h \
    ../../src/rsnapshotreportparsertest.h \
    ../../src/abstractoutputparsertest.h \
    ../../src/aptupgradeparsertest.h \
    ../../src/dfcommandparsertest.h \
    ../../src/filebackupparserabstracttest.h \
    ../../src/mockparser.h \
    ../../src/parserstestsuites.h \
    ../../src/rsynccommandparsertest.h \
    ../../src/tarcommandparsertest.h \
    ../../src/gitplumbingreportparsertest.h \
    ../../src/gitporcelainreportparsertest.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

OTHER_FILES += \
    ../../data/image.jpeg \
    ../../data/apt1.txt \
    ../../data/dummyDataFile \
    ../../data/gitadd5.log \
    ../../data/gitallchanges.log \
    ../../data/gitmod5.log \
    ../../data/gitrm5.log \
    ../../data/outputFile \
    ../../data/rsnapshot_nospaceerror.txt \
    ../../data/rsnapshot5added.log \
    ../../data/rsnapshot5changed.log \
    ../../data/rsnapshot5removed.log \
    ../../data/rsnapshotall.log \
    ../../data/DfCommand/allDrives.txt \
    ../../data/DfCommand/oneDrive.txt \
    ../../data/GitPull/gitadd5.log \
    ../../data/GitPull/gitallchanges.log \
    ../../data/GitPull/gitmod5.log \
    ../../data/GitPull/gitrm5.log \
    ../../data/GitCommit/added5.txt \
    ../../data/GitCommit/removed5.txt

CONFIG(debug, debug|release) {
	BUILD_TYPE = Debug
}
else {
	BUILD_TYPE = Release
}

TASKMANAGER_ROOT = /home/mickael/Prog/TaskManager
PARSERS_ROOT = $$TASKMANAGER_ROOT/ReportParsers
PARSERS_BIN_ROOT = $$TASKMANAGER_ROOT/bin/$$BUILD_TYPE/ReportParsers

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
