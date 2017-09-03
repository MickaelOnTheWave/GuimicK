#-------------------------------------------------
#
# Project created by QtCreator 2016-10-20T10:51:15
#
#-------------------------------------------------

QT       -= core gui

TARGET = TaskLib
TEMPLATE = lib

CONFIG += shared_and_static build_all

DEFINES += TASKLIB_LIBRARY

SOURCES += \
    changescreensaverjob.cpp \
    clamavjob.cpp \
    client.cpp \
    clientworkmanager.cpp \
    configuration.cpp \
    consolejob.cpp \
    curlconsoleemailsender.cpp \
    dummyjob.cpp \
    gitbackupjob.cpp \
    htmlreportcreator.cpp \
    jobstatus.cpp \
    linuxshutdownjob.cpp \
    profiledjob.cpp \
    shutdownjob.cpp \
    sleepjob.cpp \
    sshconsolejob.cpp \
    workresultdata.cpp \
    textreportcreator.cpp \
    abstractreportcreator.cpp \
    userconsolejob.cpp \
    rsnapshoterroranalyzer.cpp \
    wakejob.cpp \
    jobdebuginformationmanager.cpp \
    linuxfreespacecheckjob.cpp \
    rsnapshotsmartbackupjob.cpp \
    abstractjobconfiguration.cpp \
    wakejobconfiguration.cpp \
    changescreensaverjobconfiguration.cpp \
    rsnapshotbackupjobconfiguration.cpp \
    clamavjobconfiguration.cpp \
    shutdownjobconfiguration.cpp \
    userconsolejobconfiguration.cpp \
    sshconsolejobconfiguration.cpp \
    gitbackupjobconfiguration.cpp \
    diskspacecheckjobconfiguration.cpp \
    SelfIdentity.cpp \
    gitfsbackupjob.cpp \
    gitfsbackupjobconfiguration.cpp \
    abstractbackupjob.cpp \
    backupstatusmanager.cpp \
    abstractcopyfsbackupjob.cpp \
    rsynccopyfsbackupjob.cpp \
    rawcopyfsbackupjob.cpp \
    copyjobchooser.cpp \
    zipandcopyfsbackupjob.cpp \
    abstractjob.cpp \
    abstractbackupjobconfiguration.cpp \
    abstractjobdefaultconfiguration.cpp \
    rsnapshotrawbackupjob.cpp \
    rsnapshotconfigurationbuilder.cpp \
    abstractfsbackupjobconfiguration.cpp \
    copyfsbackupjobconfigurations.cpp \
    DebugOutput.cpp \
    consolereportdispatcher.cpp \
    abstractemailreportdispatcher.cpp \
    dummyemailreportdispatcher.cpp

HEADERS +=\
    abstractjob.h \
    changescreensaverjob.h \
    clamavjob.h \
    client.h \
    clientworkmanager.h \
    configuration.h \
    consolejob.h \
    curlconsoleemailsender.h \
    dummyjob.h \
    gitbackupjob.h \
    htmlreportcreator.h \
    jobstatus.h \
    linuxshutdownjob.h \
    profiledjob.h \
    SelfIdentity.h \
    shutdownjob.h \
    sleepjob.h \
    sshconsolejob.h \
    workresultdata.h \
    textreportcreator.h \
    abstractreportcreator.h \
    userconsolejob.h \
    rsnapshoterroranalyzer.h \
    wakejob.h \
    jobdebuginformationmanager.h \
    linuxfreespacecheckjob.h \
    rsnapshotsmartbackupjob.h \
    AbstractConsoleJob.h \
    abstractjobconfiguration.h \
    wakejobconfiguration.h \
    changescreensaverjobconfiguration.h \
    rsnapshotbackupjobconfiguration.h \
    clamavjobconfiguration.h \
    shutdownjobconfiguration.h \
    userconsolejobconfiguration.h \
    sshconsolejobconfiguration.h \
    gitbackupjobconfiguration.h \
    diskspacecheckjobconfiguration.h \
    gitfsbackupjob.h \
    gitfsbackupjobconfiguration.h \
    abstractbackupjob.h \
    backupstatusmanager.h \
    abstractcopyfsbackupjob.h \
    rsynccopyfsbackupjob.h \
    rawcopyfsbackupjob.h \
    copyjobchooser.h \
    zipandcopyfsbackupjob.h \
    abstractbackupjobconfiguration.h \
    abstractjobdefaultconfiguration.h \
    rsnapshotrawbackupjob.h \
    rsnapshotconfigurationbuilder.h \
    debugoutput.h \
    abstractfsbackupjobconfiguration.h \
    copyfsbackupjobconfigurations.h \
    consolereportdispatcher.h \
    AbstractReportDispatcher.h \
    abstractemailreportdispatcher.h \
    dummyemailreportdispatcher.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}


contains( CONFIG, synology ) {
	BUILD_TYPE = Synology
}
else:CONFIG(debug, debug|release) {
	BUILD_TYPE = Debug
}
else {
	BUILD_TYPE = Release
}

# linking ToolsLib
TOOLS_LIB_PATH = /home/mickael/Prog/Tools/bin/$$BUILD_TYPE/ToolsLib
TOOLS_INCLUDE_PATH = /home/mickael/Prog/Tools/ToolsLib/
LIBS += -L$$TOOLS_LIB_PATH -lToolsLib
INCLUDEPATH += $$TOOLS_INCLUDE_PATH
DEPENDPATH += $$TOOLS_INCLUDE_PATH

# linking ParsersLib
PARSERS_LIB_PATH = /home/mickael/Prog/TaskManager/bin/$$BUILD_TYPE/ReportParsers/ParsersLib/
PARSERS_INCLUDE_PATH = /home/mickael/Prog/TaskManager/ReportParsers/ParsersLib/
LIBS += -L$$PARSERS_LIB_PATH -lParsersLib
INCLUDEPATH += $$PARSERS_INCLUDE_PATH
DEPENDPATH += $$PARSERS_INCLUDE_PATH
