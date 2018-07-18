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

SOURCES += ../../src/tasktestlib.cpp \
    ../../src/gitjobtest.cpp \
    ../../src/gitrepository.cpp \
    ../../src/rsnapshoterroranalyzertest.cpp \
    ../../src/rsnapshotjobtest.cpp \
    ../../src/taskfeaturetest.cpp \
    ../../src/consolejobtest.cpp \
    ../../src/userconsolejobtest.cpp \
    ../../src/sshconsolejobtest.cpp \
    ../../src/clientworkmanagertest.cpp \
    ../../src/gitfsbackupjobtest.cpp \
    ../../src/abstractfsbackupjobtest.cpp \
    ../../src/abstractcopyfsbackupjobtest.cpp \
    ../../src/rawcopyfsbackupjobtest.cpp \
    ../../src/rsynccopyfsbackupjobtest.cpp \
    ../../src/zipandcopyfsbackupjobtest.cpp \
    ../../src/abstractjobconfigurationtest.cpp \
    ../../src/gitfsbackupjobconfigurationtest.cpp \
    ../../src/abstractbackupjobconfigurationtest.cpp \
    ../../src/abstractjobdefaultconfigurationtest.cpp \
    ../../src/rsnapshotbackupjobconfigurationtest.cpp \
    ../../src/wakejobconfigurationtest.cpp \
    ../../src/changescreensaverjobconfigurationtest.cpp \
    ../../src/clamavjobconfigurationtest.cpp \
    ../../src/shutdownjobconfigurationtest.cpp \
    ../../src/userconsolejobconfigurationtest.cpp \
    ../../src/sshconsolejobconfigurationtest.cpp \
    ../../src/gitbackupjobconfigurationtest.cpp \
    ../../src/diskspacecheckjobconfigurationtest.cpp \
    ../../src/copyfsbackupjobconfigurationstests.cpp \
    ../../src/gittesttools.cpp \
    ../../src/serverconfigurationtest.cpp \
    ../../src/remotejobsrunnerconfigurationtest.cpp \
    ../../src/wakejobtest.cpp \
    ../../src/abstractbackupjobtest.cpp \
    ../../src/tarincrementalbackupjobtest.cpp \
    ../../src/tarincrementalbackupjobconfigurationtest.cpp

HEADERS += ../../src/tasktestlib.h \
    ../../src/gitjobtest.h \
    ../../src/gitrepository.h \
    ../../src/rsnapshoterroranalyzertest.h \
    ../../src/rsnapshotjobtest.h \
    ../../src/taskfeaturetest.h \
    ../../src/consolejobtest.h \
    ../../src/userconsolejobtest.h \
    ../../src/sshconsolejobtest.h \
    ../../src/tasktestsuites.h \
    ../../src/clientworkmanagertest.h \
    ../../src/gitfsbackupjobtest.h \
    ../../src/abstractfsbackupjobtest.h \
    ../../src/abstractcopyfsbackupjobtest.h \
    ../../src/rawcopyfsbackupjobtest.h \
    ../../src/rsynccopyfsbackupjobtest.h \
    ../../src/zipandcopyfsbackupjobtest.h \
    ../../src/abstractjobconfigurationtest.h \
    ../../src/gitfsbackupjobconfigurationtest.h \
    ../../src/abstractbackupjobconfigurationtest.h \
    ../../src/abstractjobdefaultconfigurationtest.h \
    ../../src/rsnapshotbackupjobconfigurationtest.h \
    ../../src/wakejobconfigurationtest.h \
    ../../src/changescreensaverjobconfigurationtest.h \
    ../../src/clamavjobconfigurationtest.h \
    ../../src/shutdownjobconfigurationtest.h \
    ../../src/userconsolejobconfigurationtest.h \
    ../../src/sshconsolejobconfigurationtest.h \
    ../../src/gitbackupjobconfigurationtest.h \
    ../../src/diskspacecheckjobconfigurationtest.h \
    ../../src/copyfsbackupjobconfigurationstests.h \
    ../../src/gittesttools.h \
    ../../src/serverconfigurationtest.h \
    ../../src/remotejobsrunnerconfigurationtest.h \
    ../../src/wakejobtest.h \
    ../../src/abstractbackupjobtest.h \
    ../../src/tarincrementalbackupjobtest.h \
    ../../src/tarincrementalbackupjobconfigurationtest.h
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

TASKMANAGER_ROOT = /home/mickael/Prog/TaskManager

# linking TaskLib
TASK_LIB_PATH = $$TASKMANAGER_ROOT/bin/$$BUILD_TYPE/TaskLib/proj/qmake
TASK_INCLUDE_PATH = $$TASKMANAGER_ROOT/TaskLib/src
LIBS += -L$$TASK_LIB_PATH -lTaskLib
INCLUDEPATH += $$TASK_INCLUDE_PATH
DEPENDPATH += $$TASK_INCLUDE_PATH

# linking ParsersLib
PARSERS_LIB_PATH = $$TASKMANAGER_ROOT/bin/$$BUILD_TYPE/ReportParsers/ParsersLib/proj/qmake
PARSERS_INCLUDE_PATH = $$TASKMANAGER_ROOT/ReportParsers/ParsersLib/src
unix:!macx: LIBS += -L$$PARSERS_LIB_PATH -lParsersLib
INCLUDEPATH += $$PARSERS_INCLUDE_PATH
DEPENDPATH += $$PARSERS_INCLUDE_PATH

# linking ToolsLib
TOOLS_ROOT = /home/mickael/Prog/Tools
TOOLS_LIB_PATH = $$TOOLS_ROOT/bin/$$BUILD_TYPE/ToolsLib/proj/qmake
TOOLS_INCLUDE_PATH = $$TOOLS_ROOT/ToolsLib/src
LIBS += -L$$TOOLS_LIB_PATH -lToolsLib
INCLUDEPATH += $$TOOLS_INCLUDE_PATH
DEPENDPATH += $$TOOLS_INCLUDE_PATH

# linking QtTools
QTTOOLS_ROOT = /home/mickael/Prog/QtTools
QTTOOLS_LIB_PATH = $$QTTOOLS_ROOT/bin/$$BUILD_TYPE/QtTools/proj/qmake
QTTOOLS_INCLUDE_PATH = $$QTTOOLS_ROOT/QtTools/src
LIBS += -L$$QTTOOLS_LIB_PATH -lQtTools
INCLUDEPATH += $$QTTOOLS_INCLUDE_PATH
DEPENDPATH += $$QTTOOLS_INCLUDE_PATH

OTHER_FILES += \
    ../../data/RsnapshotAnalyzer/rsnapshot_invalidfolder.txt \
    ../../data/RsnapshotAnalyzer/rsnapshot_nospaceerror.txt \
    ../../data/RsnapshotAnalyzer/rsnapshotall.log \
    ../../data/Rsnapshot/rsnapshot.conf \
    ../../data/Rsnapshot/report.css \
    ../../data/TaskFeature/report.css \
    ../../data/TaskFeature/GitJobOnly/report.html \
    ../../data/TaskFeature/GitJobOnly/attachment1.txt \
    ../../data/TaskFeature/GitJobOnly/configuration.txt \
    ../../data/TaskFeature/RsnapshotNewConf/report.html \
    ../../data/TaskFeature/RsnapshotNewConf/attachment1.txt \
    ../../data/TaskFeature/RsnapshotNewConf/configuration.txt \
    ../../data/TaskFeature/RsnapshotNewConf/rsnapshot.conf \
    ../../data/TaskFeature/RsnapshotOldConf/report.html \
    ../../data/TaskFeature/RsnapshotOldConf/attachment1.txt \
    ../../data/TaskFeature/RsnapshotOldConf/configuration.txt \
    ../../data/TaskFeature/RsnapshotOldConf/rsnapshot.conf \
    ../../data/TaskFeature/ConsoleJob_NewWay/report.html \
    ../../data/TaskFeature/ConsoleJob_NewWay/attachment1.txt \
    ../../data/TaskFeature/ConsoleJob_NewWay/configuration.txt \
    ../../data/TaskFeature/ConsoleJob_OldWrongWay/report.html \
    ../../data/TaskFeature/ConsoleJob_OldWrongWay/attachment1.txt \
    ../../data/TaskFeature/ConsoleJob_OldWrongWay/configuration.txt \
    ../../data/TaskFeature/ConsoleJob_ExternalParser_BufferOutput/report.html \
    ../../data/TaskFeature/ConsoleJob_ExternalParser_BufferOutput/configuration.txt \
    ../../data/TaskFeature/ConsoleJob_ExternalParser_FileOutput/report.html \
    ../../data/TaskFeature/ConsoleJob_ExternalParser_FileOutput/configuration.txt \
    ../../data/Configuration/valid.txt \
    ../../data/Configuration/realconf.txt \
    ../../data/Configuration/root - invalid.jpg \
    ../../data/Configuration/agent - nopassword.txt \
    ../../data/Configuration/agent - unknown property.txt \
    ../../data/Configuration/client - empty job list.txt \
    ../../data/Configuration/client - noip.txt \
    ../../data/Configuration/client - no job list.txt \
    ../../data/Configuration/client - noname.txt \
    ../../data/Configuration/client - nossh.txt \
    ../../data/Configuration/client - oneonly.txt \
    ../../data/Configuration/job - unknown.txt \
    ../../data/Configuration/root - invalid boolean value.txt \
    ../../data/Configuration/root - noagent.txt \
    ../../data/Configuration/root - noclient.txt \
    ../../data/Configuration/root - unknown object.txt \
    ../../data/Configuration/root - unknown property.txt \
    ../../data/Configuration/root - unspecified report.txt \
    ../../data/Configuration/root - unsupported report.txt \
    ../../data/Configuration/agent - nopassword.txt \
    ../../data/Configuration/agent - unknown property.txt \
    ../../data/Configuration/client - empty job list.txt \
    ../../data/Configuration/client - noip.txt \
    ../../data/Configuration/client - no job list.txt \
    ../../data/Configuration/client - noname.txt \
    ../../data/Configuration/client - nossh.txt \
    ../../data/Configuration/client - oneonly.txt \
	 ../../data/Configuration/job - unknown.txt \
    ../../data/GitFsBackup/caseInitialAdd/fullReport.txt \
    ../../data/GitFsBackup/caseInitialAdd/miniDescription.txt
