TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

contains( CONFIG, UseCurlLib ) {
	DEFINES += USE_CURL_LIB
}

#contains( CONFIG, Shared ) {
#	message("Shared build")
#}
#else {
#	message("Static build")
#}

SOURCES += main.cpp \
	 emaildispatcherfactory.cpp

contains( CONFIG, UseCurlLib ) {
	SOURCES += curllibreportdispatcher.cpp
	HEADERS += curllibreportdispatcher.h
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

contains( CONFIG, UseCurlLib ) {
	# linking NetworkToolsLib
	NETWORKTOOLS_LIB_PATH = /home/mickael/Prog/Tools/bin/$$BUILD_TYPE/NetworkToolsLib/
	NETWORKTOOLS_INCLUDE_PATH = /home/mickael/Prog/Tools/NetworkToolsLib/
	LIBS += -L$$NETWORKTOOLS_LIB_PATH -lNetworkToolsLib -lcurl
	INCLUDEPATH += $$NETWORKTOOLS_INCLUDE_PATH
	DEPENDPATH += $$NETWORKTOOLS_INCLUDE_PATH
}

# linking ToolsLib
TOOLS_LIB_PATH = /home/mickael/Prog/Tools/bin/$$BUILD_TYPE/ToolsLib
TOOLS_INCLUDE_PATH = /home/mickael/Prog/Tools/ToolsLib/
LIBS += -L$$TOOLS_LIB_PATH  -lToolsLib
INCLUDEPATH += $$TOOLS_INCLUDE_PATH
DEPENDPATH += $$TOOLS_INCLUDE_PATH

OTHER_FILES += \
    data/configuration.txt \
    data/apt-upgrade.txt \
    MakeSynologyBuild.sh \
    ReleasePlan.txt \
    WorkLog.txt \
    deployNewVersion.sh \
    data/rsnapshot.conf \
    data/realconf.txt \
    data/git.txt \
    data/report.css

HEADERS += \
	 emaildispatcherfactory.h

