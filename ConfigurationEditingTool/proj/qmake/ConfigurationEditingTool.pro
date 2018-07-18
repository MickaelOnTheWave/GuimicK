#-------------------------------------------------
#
# Project created by QtCreator 2017-10-26T19:48:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=c++11

TARGET = ConfigurationEditingTool
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        ../../src/main.cpp \
        ../../src/mainwindow.cpp \
    ../../src/model.cpp \
    ../../src/configurationcheckdialog.cpp \
    ../../src/editwakejobdialog.cpp \
    ../../src/jobdatamodel.cpp \
    ../../src/abstracteditjobdialog.cpp \
    ../../src/editshutdownjobdialog.cpp \
    ../../src/editconsolejobdialog.cpp \
    ../../src/choosefiledialog.cpp \
    ../../src/abstractjobdisplay.cpp \
    ../../src/jobdelegate.cpp \
    ../../src/abstractdisplay.cpp \
    ../../src/wakejobdisplay.cpp \
    ../../src/abstractbackupjobdisplay.cpp \
    ../../src/editbackupjobdialog.cpp \
    ../../src/diskspacejobdisplay.cpp \
    ../../src/editdiskspacejobdialog.cpp \
    ../../src/editrsnapshotbackupjobdialog.cpp \
    ../../src/editbackupjobwidget.cpp \
    ../../src/editgitfsbackupjobdialog.cpp \
    ../../src/editrsyncfsbackupjobdialog.cpp \
    ../../src/jobeditdialogfactory.cpp \
    ../../src/editzipcopybackupdialog.cpp \
    ../../src/aboutdialog.cpp \
    ../../src/consolejobdisplay.cpp \
    ../../src/selectbackupfolderdialog.cpp

HEADERS += \
        ../../src/mainwindow.h \
    ../../src/model.h \
    ../../src/configurationcheckdialog.h \
    ../../src/editwakejobdialog.h \
    ../../src/jobdatamodel.h \
    ../../src/abstracteditjobdialog.h \
    ../../src/editshutdownjobdialog.h \
    ../../src/editconsolejobdialog.h \
    ../../src/choosefiledialog.h \
    ../../src/qtmetatypes.h \
    ../../src/abstractjobdisplay.h \
    ../../src/jobdelegate.h \
    ../../src/abstractdisplay.h \
    ../../src/wakejobdisplay.h \
    ../../src/abstractbackupjobdisplay.h \
    ../../src/editbackupjobdialog.h \
    ../../src/diskspacejobdisplay.h \
    ../../src/editdiskspacejobdialog.h \
    ../../src/editrsnapshotbackupjobdialog.h \
    ../../src/editbackupjobwidget.h \
    ../../src/editgitfsbackupjobdialog.h \
    ../../src/editrsyncfsbackupjobdialog.h \
    ../../src/jobeditdialogfactory.h \
    ../../src/editzipcopybackupdialog.h \
    ../../src/aboutdialog.h \
    ../../src/consolejobdisplay.h \
    ../../src/selectbackupfolderdialog.h

FORMS += \
        ../../src/mainwindow.ui \
    ../../src/configurationcheckdialog.ui \
    ../../src/editwakejobdialog.ui \
    ../../src/editshutdownjobdialog.ui \
    ../../src/editconsolejobdialog.ui \
    ../../src/choosefiledialog.ui \
    ../../src/abstractjobdisplay.ui \
    ../../src/wakejobdisplay.ui \
    ../../src/abstractbackupjobdisplay.ui \
    ../../src/editbackupjobdialog.ui \
    ../../src/diskspacejobdisplay.ui \
    ../../src/editdiskspacejobdialog.ui \
    ../../src/editrsnapshotbackupjobdialog.ui \
    ../../src/editbackupjobwidget.ui \
    ../../src/editgitfsbackupjobdialog.ui \
    ../../src/editrsyncfsbackupjobdialog.ui \
    ../../src/editzipcopybackupdialog.ui \
    ../../src/aboutdialog.ui \
    ../../src/consolejobdisplay.ui \
    ../../src/selectbackupfolderdialog.ui

contains( CONFIG, synology ) {
   BUILD_TYPE = Synology
}
else:CONFIG(debug, debug|release) {
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
LIBS += -L$$PARSERS_LIB_PATH -lParsersLib
INCLUDEPATH += $$PARSERS_INCLUDE_PATH
DEPENDPATH += $$PARSERS_INCLUDE_PATH

# linking ToolsLib
TOOLS_ROOT = /home/mickael/Prog/Tools
TOOLSLIB_ROOT = $$TOOLS_ROOT/ToolsLib
TOOLS_LIB_PATH = $$TOOLS_ROOT/bin/$$BUILD_TYPE/ToolsLib/proj/qmake
TOOLS_INCLUDE_PATH = $$TOOLSLIB_ROOT/src
LIBS += -L$$TOOLS_LIB_PATH -lToolsLib
INCLUDEPATH += $$TOOLS_INCLUDE_PATH
DEPENDPATH += $$TOOLS_INCLUDE_PATH
