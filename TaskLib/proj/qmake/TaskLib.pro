#-------------------------------------------------
#
# Project created by QtCreator 2016-10-20T10:51:15
#
#-------------------------------------------------

QT       -= core gui

TARGET = TaskLib
TEMPLATE = lib

contains( CONFIG, UseCurlLib ) {
   CONFIG += staticlib
}
else {
   CONFIG += shared_and_static build_all
}

DEFINES += TASKLIB_LIBRARY

SOURCES += \
    ../../src/changescreensaverjob.cpp \
    ../../src/clamavjob.cpp \
    ../../src/client.cpp \
    ../../src/clientworkmanager.cpp \
    ../../src/consolejob.cpp \
    ../../src/curlconsoleemailsender.cpp \
    ../../src/dummyjob.cpp \
    ../../src/gitbackupjob.cpp \
    ../../src/htmlreportcreator.cpp \
    ../../src/jobstatus.cpp \
    ../../src/linuxshutdownjob.cpp \
    ../../src/profiledjob.cpp \
    ../../src/shutdownjob.cpp \
    ../../src/sleepjob.cpp \
    ../../src/sshconsolejob.cpp \
    ../../src/workresultdata.cpp \
    ../../src/textreportcreator.cpp \
    ../../src/abstractreportcreator.cpp \
    ../../src/userconsolejob.cpp \
    ../../src/rsnapshoterroranalyzer.cpp \
    ../../src/jobdebuginformationmanager.cpp \
    ../../src/linuxfreespacecheckjob.cpp \
    ../../src/rsnapshotsmartbackupjob.cpp \
    ../../src/abstractjobconfiguration.cpp \
    ../../src/wakejobconfiguration.cpp \
    ../../src/changescreensaverjobconfiguration.cpp \
    ../../src/rsnapshotbackupjobconfiguration.cpp \
    ../../src/clamavjobconfiguration.cpp \
    ../../src/shutdownjobconfiguration.cpp \
    ../../src/userconsolejobconfiguration.cpp \
    ../../src/sshconsolejobconfiguration.cpp \
    ../../src/gitbackupjobconfiguration.cpp \
    ../../src/diskspacecheckjobconfiguration.cpp \
    ../../src/SelfIdentity.cpp \
    ../../src/gitfsbackupjob.cpp \
    ../../src/gitfsbackupjobconfiguration.cpp \
    ../../src/abstractbackupjob.cpp \
    ../../src/backupstatusmanager.cpp \
    ../../src/abstractcopyfsbackupjob.cpp \
    ../../src/rsynccopyfsbackupjob.cpp \
    ../../src/rawcopyfsbackupjob.cpp \
    ../../src/copyjobchooser.cpp \
    ../../src/zipandcopyfsbackupjob.cpp \
    ../../src/abstractjob.cpp \
    ../../src/abstractbackupjobconfiguration.cpp \
    ../../src/abstractjobdefaultconfiguration.cpp \
    ../../src/rsnapshotrawbackupjob.cpp \
    ../../src/rsnapshotconfigurationbuilder.cpp \
    ../../src/abstractfsbackupjobconfiguration.cpp \
    ../../src/copyfsbackupjobconfigurations.cpp \
    ../../src/DebugOutput.cpp \
    ../../src/consolereportdispatcher.cpp \
    ../../src/abstractemailreportdispatcher.cpp \
    ../../src/dummyemailreportdispatcher.cpp \
    ../../src/abstractstructuredreportcreator.cpp \
    ../../src/maintoolmodule.cpp \
    ../../src/filereportdispatcher.cpp \
    ../../src/gitcommontools.cpp \
    ../../src/remotejobsrunner.cpp \
    ../../src/taskmanagerconfiguration.cpp \
    ../../src/serverconfiguration.cpp \
    ../../src/clientjobsconfiguration.cpp \
    ../../src/clientconfiguration.cpp \
    ../../src/resultcollectionstatus.cpp \
    ../../src/remotejobsrunnerconfiguration.cpp \
    ../../src/AbstractConsoleJob.cpp \
    ../../src/jobfactory.cpp \
    ../../src/tarincrementalbackup.cpp \
    ../../src/tartools.cpp \
    ../../src/tarincrementalbackupjobconfiguration.cpp \
    ../../src/abstractwakejob.cpp \
    ../../src/libwakejob.cpp \
    ../../src/consolewakejob.cpp

HEADERS +=\
    ../../src/abstractjob.h \
    ../../src/changescreensaverjob.h \
    ../../src/clamavjob.h \
    ../../src/client.h \
    ../../src/clientworkmanager.h \
    ../../src/consolejob.h \
    ../../src/curlconsoleemailsender.h \
    ../../src/dummyjob.h \
    ../../src/gitbackupjob.h \
    ../../src/htmlreportcreator.h \
    ../../src/jobstatus.h \
    ../../src/linuxshutdownjob.h \
    ../../src/profiledjob.h \
    ../../src/SelfIdentity.h \
    ../../src/shutdownjob.h \
    ../../src/sleepjob.h \
    ../../src/sshconsolejob.h \
    ../../src/workresultdata.h \
    ../../src/textreportcreator.h \
    ../../src/abstractreportcreator.h \
    ../../src/userconsolejob.h \
    ../../src/rsnapshoterroranalyzer.h \
    ../../src/jobdebuginformationmanager.h \
    ../../src/linuxfreespacecheckjob.h \
    ../../src/rsnapshotsmartbackupjob.h \
    ../../src/AbstractConsoleJob.h \
    ../../src/abstractjobconfiguration.h \
    ../../src/wakejobconfiguration.h \
    ../../src/changescreensaverjobconfiguration.h \
    ../../src/rsnapshotbackupjobconfiguration.h \
    ../../src/clamavjobconfiguration.h \
    ../../src/shutdownjobconfiguration.h \
    ../../src/userconsolejobconfiguration.h \
    ../../src/sshconsolejobconfiguration.h \
    ../../src/gitbackupjobconfiguration.h \
    ../../src/diskspacecheckjobconfiguration.h \
    ../../src/gitfsbackupjob.h \
    ../../src/gitfsbackupjobconfiguration.h \
    ../../src/abstractbackupjob.h \
    ../../src/backupstatusmanager.h \
    ../../src/abstractcopyfsbackupjob.h \
    ../../src/rsynccopyfsbackupjob.h \
    ../../src/rawcopyfsbackupjob.h \
    ../../src/copyjobchooser.h \
    ../../src/zipandcopyfsbackupjob.h \
    ../../src/abstractbackupjobconfiguration.h \
    ../../src/abstractjobdefaultconfiguration.h \
    ../../src/rsnapshotrawbackupjob.h \
    ../../src/rsnapshotconfigurationbuilder.h \
    ../../src/debugoutput.h \
    ../../src/abstractfsbackupjobconfiguration.h \
    ../../src/copyfsbackupjobconfigurations.h \
    ../../src/consolereportdispatcher.h \
    ../../src/AbstractReportDispatcher.h \
    ../../src/abstractemailreportdispatcher.h \
    ../../src/dummyemailreportdispatcher.h \
    ../../src/abstractstructuredreportcreator.h \
    ../../src/maintoolmodule.h \
    ../../src/abstractreportdispatcherreplacer.h \
    ../../src/filereportdispatcher.h \
    ../../src/gitcommontools.h \
    ../../src/remotejobsrunner.h \
    ../../src/taskmanagerconfiguration.h \
    ../../src/serverconfiguration.h \
    ../../src/clientjobsconfiguration.h \
    ../../src/clientconfiguration.h \
    ../../src/resultcollectionstatus.h \
    ../../src/remotejobsrunnerconfiguration.h \
    ../../src/jobfactory.h \
    ../../src/tarincrementalbackup.h \
    ../../src/tartools.h \
    ../../src/jobexecutiontarget.h \
    ../../src/tarincrementalbackupjobconfiguration.h \
    ../../src/abstractwakejob.h \
    ../../src/libwakejob.h \
    ../../src/consolewakejob.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}


isEmpty(BUILD_TYPE){
   contains( CONFIG, synology ) {
      BUILD_TYPE = Synology
   }
   else:CONFIG(debug, debug|release) {
      BUILD_TYPE = Debug
   }
   else {
      BUILD_TYPE = Release
   }
}

# linking ToolsLib
TOOLS_ROOT = /home/mickael/Prog/Tools
TOOLSLIB_ROOT = $$TOOLS_ROOT/ToolsLib
TOOLS_LIB_PATH = $$TOOLS_ROOT/bin/$$BUILD_TYPE/ToolsLib/proj/qmake
TOOLS_INCLUDE_PATH = $$TOOLSLIB_ROOT/src
LIBS += -L$$TOOLS_LIB_PATH -lToolsLib
INCLUDEPATH += $$TOOLS_INCLUDE_PATH
DEPENDPATH += $$TOOLS_INCLUDE_PATH

# linking ParsersLib
TASKMANAGER_ROOT = /home/mickael/Prog/TaskManager
PARSERS_LIB_PATH = $$TASKMANAGER_ROOT/bin/$$BUILD_TYPE/ReportParsers/ParsersLib/proj/qmake
PARSERS_INCLUDE_PATH = $$TASKMANAGER_ROOT/ReportParsers/ParsersLib/src
LIBS += -L$$PARSERS_LIB_PATH -lParsersLib
INCLUDEPATH += $$PARSERS_INCLUDE_PATH
DEPENDPATH += $$PARSERS_INCLUDE_PATH
