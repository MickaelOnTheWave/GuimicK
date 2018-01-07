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
        main.cpp \
        mainwindow.cpp \
    model.cpp \
    configurationcheckdialog.cpp \
    editwakejobdialog.cpp \
    jobdatamodel.cpp \
    abstracteditjobdialog.cpp \
    editshutdownjobdialog.cpp \
    editconsolejobdialog.cpp \
    choosefiledialog.cpp \
    abstractjobdisplay.cpp \
    jobdelegate.cpp \
    abstractdisplay.cpp \
    wakejobdisplay.cpp \
    abstractbackupjobdisplay.cpp \
    editbackupjobdialog.cpp \
    diskspacejobdisplay.cpp \
    editdiskspacejobdialog.cpp \
    editrsnapshotbackupjobdialog.cpp \
    editbackupjobwidget.cpp \
    editgitfsbackupjobdialog.cpp \
    editrsyncfsbackupjobdialog.cpp \
    jobeditdialogfactory.cpp \
    editzipcopybackupdialog.cpp

HEADERS += \
        mainwindow.h \
    model.h \
    configurationcheckdialog.h \
    editwakejobdialog.h \
    jobdatamodel.h \
    abstracteditjobdialog.h \
    editshutdownjobdialog.h \
    editconsolejobdialog.h \
    choosefiledialog.h \
    qtmetatypes.h \
    abstractjobdisplay.h \
    jobdelegate.h \
    abstractdisplay.h \
    wakejobdisplay.h \
    abstractbackupjobdisplay.h \
    editbackupjobdialog.h \
    diskspacejobdisplay.h \
    editdiskspacejobdialog.h \
    editrsnapshotbackupjobdialog.h \
    editbackupjobwidget.h \
    editgitfsbackupjobdialog.h \
    editrsyncfsbackupjobdialog.h \
    jobeditdialogfactory.h \
    editzipcopybackupdialog.h

FORMS += \
        mainwindow.ui \
    configurationcheckdialog.ui \
    editwakejobdialog.ui \
    editshutdownjobdialog.ui \
    editconsolejobdialog.ui \
    choosefiledialog.ui \
    abstractjobdisplay.ui \
    wakejobdisplay.ui \
    abstractbackupjobdisplay.ui \
    editbackupjobdialog.ui \
    diskspacejobdisplay.ui \
    editdiskspacejobdialog.ui \
    editrsnapshotbackupjobdialog.ui \
    editbackupjobwidget.ui \
    editgitfsbackupjobdialog.ui \
    editrsyncfsbackupjobdialog.ui \
    editzipcopybackupdialog.ui

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

# linking ToolsLib
TOOLS_LIB_PATH = /home/mickael/Prog/Tools/bin/$$BUILD_TYPE/ToolsLib
TOOLS_INCLUDE_PATH = /home/mickael/Prog/Tools/ToolsLib/
LIBS += -L$$TOOLS_LIB_PATH  -lToolsLib
INCLUDEPATH += $$TOOLS_INCLUDE_PATH
DEPENDPATH += $$TOOLS_INCLUDE_PATH
