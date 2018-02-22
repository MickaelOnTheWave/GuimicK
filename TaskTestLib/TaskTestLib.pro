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
    rsnapshoterroranalyzertest.cpp \
    rsnapshotjobtest.cpp \
    taskfeaturetest.cpp \
    consolejobtest.cpp \
    userconsolejobtest.cpp \
    sshconsolejobtest.cpp \
    clientworkmanagertest.cpp \
    gitfsbackupjobtest.cpp \
    abstractfsbackupjobtest.cpp \
    abstractcopyfsbackupjobtest.cpp \
    rawcopyfsbackupjobtest.cpp \
    rsynccopyfsbackupjobtest.cpp \
    zipandcopyfsbackupjobtest.cpp \
    abstractjobconfigurationtest.cpp \
    gitfsbackupjobconfigurationtest.cpp \
    abstractbackupjobconfigurationtest.cpp \
    abstractjobdefaultconfigurationtest.cpp \
    rsnapshotbackupjobconfigurationtest.cpp \
    wakejobconfigurationtest.cpp \
    changescreensaverjobconfigurationtest.cpp \
    clamavjobconfigurationtest.cpp \
    shutdownjobconfigurationtest.cpp \
    userconsolejobconfigurationtest.cpp \
    sshconsolejobconfigurationtest.cpp \
    gitbackupjobconfigurationtest.cpp \
    diskspacecheckjobconfigurationtest.cpp \
    copyfsbackupjobconfigurationstests.cpp \
    gittesttools.cpp \
    serverconfigurationtest.cpp \
    remotejobsrunnerconfigurationtest.cpp \
    wakejobtest.cpp \
    abstractbackupjobtest.cpp \
    tarincrementalbackupjobtest.cpp

HEADERS += tasktestlib.h \
    gitjobtest.h \
    gitrepository.h \
    rsnapshoterroranalyzertest.h \
    rsnapshotjobtest.h \
    taskfeaturetest.h \
    filetestutils.h \
    consolejobtest.h \
    userconsolejobtest.h \
    sshconsolejobtest.h \
    tasktestsuites.h \
    clientworkmanagertest.h \
    gitfsbackupjobtest.h \
    abstractfsbackupjobtest.h \
    abstractcopyfsbackupjobtest.h \
    rawcopyfsbackupjobtest.h \
    rsynccopyfsbackupjobtest.h \
    zipandcopyfsbackupjobtest.h \
    abstractjobconfigurationtest.h \
    gitfsbackupjobconfigurationtest.h \
    abstractbackupjobconfigurationtest.h \
    abstractjobdefaultconfigurationtest.h \
    rsnapshotbackupjobconfigurationtest.h \
    wakejobconfigurationtest.h \
    changescreensaverjobconfigurationtest.h \
    clamavjobconfigurationtest.h \
    shutdownjobconfigurationtest.h \
    userconsolejobconfigurationtest.h \
    sshconsolejobconfigurationtest.h \
    gitbackupjobconfigurationtest.h \
    diskspacecheckjobconfigurationtest.h \
    copyfsbackupjobconfigurationstests.h \
    gittesttools.h \
    serverconfigurationtest.h \
    remotejobsrunnerconfigurationtest.h \
    wakejobtest.h \
    abstractbackupjobtest.h \
    tarincrementalbackupjobtest.h
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

# linking QtTools
QTTOOLS_LIB_PATH = /home/mickael/Prog/QtTools/bin/$$BUILD_TYPE/QtTools/
QTTOOLS_INCLUDE_PATH = /home/mickael/Prog/QtTools/QtTools
LIBS += -L$$QTTOOLS_LIB_PATH -lQtTools
INCLUDEPATH += $$QTTOOLS_INCLUDE_PATH
DEPENDPATH += $$QTTOOLS_INCLUDE_PATH

OTHER_FILES += \
    data/RsnapshotAnalyzer/rsnapshot_invalidfolder.txt \
    data/RsnapshotAnalyzer/rsnapshot_nospaceerror.txt \
    data/RsnapshotAnalyzer/rsnapshotall.log \
    data/Rsnapshot/rsnapshot.conf \
    data/Rsnapshot/report.css \
    data/TaskFeature/report.css \
    data/TaskFeature/GitJobOnly/report.html \
    data/TaskFeature/GitJobOnly/attachment1.txt \
    data/TaskFeature/GitJobOnly/configuration.txt \
    data/TaskFeature/RsnapshotNewConf/report.html \
    data/TaskFeature/RsnapshotNewConf/attachment1.txt \
    data/TaskFeature/RsnapshotNewConf/configuration.txt \
    data/TaskFeature/RsnapshotNewConf/rsnapshot.conf \
    data/TaskFeature/RsnapshotOldConf/report.html \
    data/TaskFeature/RsnapshotOldConf/attachment1.txt \
    data/TaskFeature/RsnapshotOldConf/configuration.txt \
    data/TaskFeature/RsnapshotOldConf/rsnapshot.conf \
    data/TaskFeature/ConsoleJob_NewWay/report.html \
    data/TaskFeature/ConsoleJob_NewWay/attachment1.txt \
    data/TaskFeature/ConsoleJob_NewWay/configuration.txt \
    data/TaskFeature/ConsoleJob_OldWrongWay/report.html \
    data/TaskFeature/ConsoleJob_OldWrongWay/attachment1.txt \
    data/TaskFeature/ConsoleJob_OldWrongWay/configuration.txt \
    data/TaskFeature/ConsoleJob_ExternalParser_BufferOutput/report.html \
    data/TaskFeature/ConsoleJob_ExternalParser_BufferOutput/configuration.txt \
    data/TaskFeature/ConsoleJob_ExternalParser_FileOutput/report.html \
    data/TaskFeature/ConsoleJob_ExternalParser_FileOutput/configuration.txt \
    data/Configuration/valid.txt \
    data/Configuration/realconf.txt \
    data/Configuration/root - invalid.jpg \
    data/Configuration/agent - nopassword.txt \
    data/Configuration/agent - unknown property.txt \
    data/Configuration/client - empty job list.txt \
    data/Configuration/client - noip.txt \
    data/Configuration/client - no job list.txt \
    data/Configuration/client - noname.txt \
    data/Configuration/client - nossh.txt \
    data/Configuration/client - oneonly.txt \
    data/Configuration/job - unknown.txt \
    data/Configuration/root - invalid boolean value.txt \
    data/Configuration/root - noagent.txt \
    data/Configuration/root - noclient.txt \
    data/Configuration/root - unknown object.txt \
    data/Configuration/root - unknown property.txt \
    data/Configuration/root - unspecified report.txt \
    data/Configuration/root - unsupported report.txt \
    data/Configuration/agent - nopassword.txt \
    data/Configuration/agent - unknown property.txt \
    data/Configuration/client - empty job list.txt \
    data/Configuration/client - noip.txt \
    data/Configuration/client - no job list.txt \
    data/Configuration/client - noname.txt \
    data/Configuration/client - nossh.txt \
    data/Configuration/client - oneonly.txt \
	 data/Configuration/job - unknown.txt \
    data/GitFsBackup/caseInitialAdd/fullReport.txt \
    data/GitFsBackup/caseInitialAdd/miniDescription.txt
