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
    main.cpp \
    gitreportparsertest.cpp \
    rsnapshotreportparsertest.cpp \
    abstractoutputparsertest.cpp \
    mockparser.cpp \
    filebackupparserabstracttest.cpp \
    aptupgradeparsertest.cpp \
    dfcommandparsertest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

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

INCLUDEPATH += $$TOOLS_INCLUDE_PATH
DEPENDPATH += $$TOOLS_INCLUDE_PATH

OTHER_FILES += \
    data/image.jpeg \
    data/gitadd5.log \
    data/gitallchanges.log \
    data/gitmod5.log \
    data/gitrm5.log \
    data/rsnapshot5added.log \
    data/rsnapshot5changed.log \
    data/rsnapshot5removed.log \
    data/rsnapshotall.log \
    data/apt1.txt \
    data/rsnapshot_nospaceerror.txt \
    data/DfCommand/allDrives.txt \
    data/DfCommand/oneDrive.txt

HEADERS += \
    gitreportparsertest.h \
    rsnapshotreportparsertest.h \
    abstractoutputparsertest.h \
    mockparser.h \
    filebackupparserabstracttest.h \
    aptupgradeparsertest.h \
    dfcommandparsertest.h
