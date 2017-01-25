#!/bin/sh

TARGET_DIRECTORY="/home/mickael/Prog/TaskManager/bin/SynologyPackage/"
TOOLS_BUILD_PATH="/home/mickael/Prog/Tools/bin/Synology/"
TOOLSLIB_BIN_PATH=$TOOLS_BUILD_PATH"ToolsLib/"
TOOLSLIB_PATH="/home/mickael/Prog/Tools/ToolsLib/"
TASKMANAGER_BUILD_PATH="/home/mickael/Prog/TaskManager/bin/Synology/"
PARSERSLIB_BIN_PATH1="/home/mickael/Prog/TaskManager/bin/Synology/ReportParsers/"
PARSERSLIB_BIN_PATH=$PARSERSLIB_BIN_PATH1"ParsersLib/"
PARSERSLIB_PATH="/home/mickael/Prog/TaskManager/ReportParsers/ParsersLib/"
TASKLIB_BIN_PATH="/home/mickael/Prog/TaskManager/bin/Synology/TaskLib"
TASKLIB_PATH="/home/mickael/Prog/TaskManager/TaskLib/"
TASKTOOL_BIN_PATH="/home/mickael/Prog/TaskManager/bin/Synology/TaskTool"
TASKTOOL_PATH="/home/mickael/Prog/TaskManager/TaskTool/"
PARSERTOOL_BIN_PATH="/home/mickael/Prog/TaskManager/bin/Synology/ReportParsers/CommandLineTool/"
PARSERTOOL_PATH="/home/mickael/Prog/TaskManager/ReportParsers/CommandLineTool/"

if [ ! -d "$TOOLS_BUILD_PATH" ]; then
  mkdir $TOOLS_BUILD_PATH
fi

if [ ! -d "$TASKMANAGER_BUILD_PATH" ]; then
  mkdir $TASKMANAGER_BUILD_PATH
fi


# Building Tools lib
if [ ! -d "$TOOLSLIB_BIN_PATH" ]; then
  mkdir $TOOLSLIB_BIN_PATH
fi
cd $TOOLSLIB_BIN_PATH
qmake -spec linux-arm-synology -o toolsMakefile "CONFIG+=synology" $TOOLSLIB_PATH/ToolsLib.pro
make -f toolsMakefile

# Building Parsers lib
if [ ! -d "$PARSERSLIB_BIN_PATH1" ]; then
  mkdir $PARSERSLIB_BIN_PATH1
fi
if [ ! -d "$PARSERSLIB_BIN_PATH" ]; then
  mkdir $PARSERSLIB_BIN_PATH
fi
cd $PARSERSLIB_BIN_PATH
qmake -spec linux-arm-synology -o parsersMakefile "CONFIG+=synology" $PARSERSLIB_PATH/ParsersLib.pro
make -f parsersMakefile

# Building Task lib
if [ ! -d "$TASKLIB_BIN_PATH" ]; then
  mkdir $TASKLIB_BIN_PATH
fi
cd $TASKLIB_BIN_PATH
qmake -spec linux-arm-synology -o taskMakefile "CONFIG+=synology" $TASKLIB_PATH/TaskLib.pro
make -f taskMakefile

# Building Task tool
if [ ! -d "$TASKTOOL_BIN_PATH" ]; then
  mkdir $TASKTOOL_BIN_PATH
fi
cd $TASKTOOL_BIN_PATH
qmake -spec linux-arm-synology -o execMakefile "CONFIG+=synology" $TASKTOOL_PATH/TaskTool.pro
make -f execMakefile

# Building Parser tool
if [ ! -d "$PARSERTOOL_BIN_PATH" ]; then
  mkdir $PARSERTOOL_BIN_PATH
fi
cd $PARSERTOOL_BIN_PATH
qmake -spec linux-arm-synology -o parserToolMakefile "CONFIG+=synology" $PARSERTOOL_PATH/CommandLineTool.pro
make -f parserToolMakefile

# Copy everything into synology folder, ready for transfer
if [ -d "$TARGET_DIRECTORY" ]; then
  rm -rf $TARGET_DIRECTORY
fi

mkdir $TARGET_DIRECTORY

cp $TOOLSLIB_BIN_PATH/libToolsLib.so $TARGET_DIRECTORY/
cp $PARSERSLIB_BIN_PATH/libParsersLib.so $TARGET_DIRECTORY/
cp $TASKLIB_BIN_PATH/libTaskLib.so $TARGET_DIRECTORY/
cp $TASKTOOL_BIN_PATH/TaskTool $TARGET_DIRECTORY/
cp $PARSERTOOL_BIN_PATH/CommandLineTool $TARGET_DIRECTORY/

