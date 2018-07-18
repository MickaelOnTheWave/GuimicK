TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

contains( CONFIG, UseCurlLib ) {
	DEFINES += USE_CURL_LIB
}
else {
	QMAKE_LFLAGS += -static
}

SOURCES += ../../src/main.cpp \
	 ../../src/emaildispatcherfactory.cpp \
    ../../src/curldispatcherreplacer.cpp

contains( CONFIG, UseCurlLib ) {
	SOURCES += curllibreportdispatcher.cpp
	HEADERS += curllibreportdispatcher.h
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

ROOTPATH = /home/mickael/Prog
TASKMANAGERPATH = $$ROOTPATH/TaskManager
TOOLSPATH = $$ROOTPATH/Tools

# linking TaskLib
TASK_LIB_PATH = $$TASKMANAGERPATH/bin/$$BUILD_TYPE/TaskLib/proj/qmake
TASK_INCLUDE_PATH = $$TASKMANAGERPATH/TaskLib/src
LIBS += -L$$TASK_LIB_PATH -lTaskLib
INCLUDEPATH += $$TASK_INCLUDE_PATH
DEPENDPATH += $$TASK_INCLUDE_PATH

# linking ParsersLib
PARSERS_LIB_PATH = $$TASKMANAGERPATH/bin/$$BUILD_TYPE/ReportParsers/ParsersLib/proj/qmake
PARSERS_INCLUDE_PATH = $$TASKMANAGERPATH/ReportParsers/ParsersLib/src
unix:!macx: LIBS += -L$$PARSERS_LIB_PATH -lParsersLib
INCLUDEPATH += $$PARSERS_INCLUDE_PATH
DEPENDPATH += $$PARSERS_INCLUDE_PATH

# linking ToolsLib
TOOLS_ROOT = /home/mickael/Prog/Tools
TOOLSLIB_ROOT = $$TOOLS_ROOT/ToolsLib/
TOOLS_LIB_PATH = $$TOOLS_ROOT/bin/$$BUILD_TYPE/ToolsLib/proj/qmake
TOOLS_INCLUDE_PATH = $$TOOLSLIB_ROOT/src
LIBS += -L$$TOOLS_LIB_PATH -lToolsLib
INCLUDEPATH += $$TOOLS_INCLUDE_PATH
DEPENDPATH += $$TOOLS_INCLUDE_PATH


contains( CONFIG, UseCurlLib ) {
	# linking NetworkToolsLib
   NETWORKTOOLS_LIB_PATH = $$TOOLSPATH/bin/$$BUILD_TYPE/NetworkToolsLib
   NETWORKTOOLS_INCLUDE_PATH = $$TOOLSPATH/NetworkToolsLib/
   LIBS += -L$$NETWORKTOOLS_LIB_PATH -lNetworkToolsLib #-lcurl
	INCLUDEPATH += $$NETWORKTOOLS_INCLUDE_PATH
	DEPENDPATH += $$NETWORKTOOLS_INCLUDE_PATH
}


OTHER_FILES += \
    ../../data/configuration.txt \
    ../../data/apt-upgrade.txt \
    ../../MakeSynologyBuild.sh \
    ../../ReleasePlan.txt \
    ../../WorkLog.txt \
    ../../deployNewVersion.sh \
    ../../data/rsnapshot.conf \
    ../../data/realconf.txt \
    ../../data/git.txt \
    ../../data/report.css \
    ../../data/currentProductionConfig.txt

HEADERS += \
	 emaildispatcherfactory.h \
    curldispatcherreplacer.h

