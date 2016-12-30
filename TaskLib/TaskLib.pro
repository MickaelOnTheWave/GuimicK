#-------------------------------------------------
#
# Project created by QtCreator 2016-10-20T10:51:15
#
#-------------------------------------------------

QT       -= core gui

TARGET = TaskLib
TEMPLATE = lib

DEFINES += TASKLIB_LIBRARY

SOURCES += \
    changescreensaverjob.cpp \
    clamavjob.cpp \
    client.cpp \
    clientworkmanager.cpp \
    configuration.cpp \
    consolejob.cpp \
    consolewakejob.cpp \
    curlconsoleemailsender.cpp \
    dummyjob.cpp \
    emailreportdispatcher.cpp \
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
    rsnapshotbackupjob.cpp

HEADERS +=\
    abstractjob.h \
    changescreensaverjob.h \
    clamavjob.h \
    client.h \
    clientworkmanager.h \
    configuration.h \
    consolejob.h \
    consolewakejob.h \
    curlconsoleemailsender.h \
    dummyjob.h \
    emailreportdispatcher.h \
    gitbackupjob.h \
    htmlreportcreator.h \
    jobstatus.h \
    linuxshutdownjob.h \
    profiledjob.h \
    ReportDispatcher.h \
    SelfIdentity.h \
    shutdownjob.h \
    sleepjob.h \
    sshconsolejob.h \
    workresultdata.h \
    textreportcreator.h \
    abstractreportcreator.h \
    userconsolejob.h \
    rsnapshotbackupjob.h

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
