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

SOURCES += parserstestlib.cpp \
    rsnapshotreportparsertest.cpp \
    abstractoutputparsertest.cpp \
    aptupgradeparsertest.cpp \
    dfcommandparsertest.cpp \
    filebackupparserabstracttest.cpp \
    gitreportparsertest.cpp \
    mockparser.cpp \
    rsynccommandparsertest.cpp \
    tarcommandparsertest.cpp \
    gitplumbingreportparsertest.cpp

HEADERS += parserstestlib.h \
    rsnapshotreportparsertest.h \
    abstractoutputparsertest.h \
    aptupgradeparsertest.h \
    dfcommandparsertest.h \
    filebackupparserabstracttest.h \
    gitreportparsertest.h \
    mockparser.h \
    parserstestsuites.h \
    rsynccommandparsertest.h \
    tarcommandparsertest.h \
    gitplumbingreportparsertest.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

OTHER_FILES += \
    data/image.jpeg \
    data/apt1.txt \
    data/dummyDataFile \
    data/gitadd5.log \
    data/gitallchanges.log \
    data/gitmod5.log \
    data/gitrm5.log \
    data/outputFile \
    data/rsnapshot_nospaceerror.txt \
    data/rsnapshot5added.log \
    data/rsnapshot5changed.log \
    data/rsnapshot5removed.log \
    data/rsnapshotall.log \
    data/DfCommand/allDrives.txt \
    data/DfCommand/oneDrive.txt \
    data/GitPull/gitadd5.log \
    data/GitPull/gitallchanges.log \
    data/GitPull/gitmod5.log \
    data/GitPull/gitrm5.log \
    data/GitCommit/added5.txt \
    data/GitCommit/removed5.txt

CONFIG(debug, debug|release) {
	BUILD_TYPE = Debug
}
else {
	BUILD_TYPE = Release
}

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
