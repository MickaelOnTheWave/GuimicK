#-------------------------------------------------
#
# Project created by QtCreator 2017-02-13T19:17:21
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = TaskTestLib
TEMPLATE = lib
#CONFIG += staticlib

QMAKE_CXXFLAGS += -std=c++11

SOURCES += tasktestlib.cpp \
    gitjobtest.cpp \
    gitrepository.cpp \
    gittools.cpp \
    rsnapshoterroranalyzertest.cpp \
    rsnapshotjobtest.cpp \
    taskfeaturetest.cpp \
    filetestutils.cpp

HEADERS += tasktestlib.h \
    gitjobtest.h \
    gitrepository.h \
    gittools.h \
    rsnapshoterroranalyzertest.h \
    rsnapshotjobtest.h \
    taskfeaturetest.h \
    filetestutils.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

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
TOOLS_LIB_PATH = /home/mickael/Prog/Tools/bin/$$BUILD_TYPE/ToolsLib
TOOLS_INCLUDE_PATH = /home/mickael/Prog/Tools/ToolsLib/
LIBS += -L$$TOOLS_LIB_PATH -lToolsLib
INCLUDEPATH += $$TOOLS_INCLUDE_PATH
DEPENDPATH += $$TOOLS_INCLUDE_PATH

OTHER_FILES += \
    data/RsnapshotAnalyzer/rsnapshot_invalidfolder.txt \
    data/RsnapshotAnalyzer/rsnapshot_nospaceerror.txt \
    data/RsnapshotAnalyzer/rsnapshotall.log \
    data/Rsnapshot/rsnapshot.conf \
    data/Rsnapshot/report.css \
    data/TaskFeature/report.css \
    data/TaskFeature/GitJobOnly/report.html \
    data/TaskFeature/GitJobOnly/attachment1.txt \
    data/TaskFeature/GitJobOnly/configuration.txt
