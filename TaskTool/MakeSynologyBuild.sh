#!/bin/sh

TARGET_DIRECTORY="/home/mickael/Prog/TaskManager/bin/SynologyPackage/"

TOOLS_BUILD_PATH="/home/mickael/Prog/Tools/bin/Synology/"
TOOLSLIB_BIN_PATH=$TOOLS_BUILD_PATH"ToolsLib/"
TOOLSLIB_PATH="/home/mickael/Prog/Tools/ToolsLib/"

NETWORKTOOLSLIB_BIN_PATH=$TOOLS_BUILD_PATH"NetworkToolsLib/"
NETWORKTOOLSLIB_PATH="/home/mickael/Prog/Tools/NetworkToolsLib/"


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

createFolderIfNotPresent()
{
	FOLDER=$1
	if [ ! -d "$FOLDER" ]; then
	mkdir $FOLDER
	fi
}

createFolderIfNotPresent $TOOLS_BUILD_PATH
createFolderIfNotPresent $TASKMANAGER_BUILD_PATH

# Building Tools lib
createFolderIfNotPresent $TOOLSLIB_BIN_PATH
cd $TOOLSLIB_BIN_PATH
qmake -spec linux-arm-synology -o toolsMakefile "CONFIG+=synology" $TOOLSLIB_PATH/ToolsLib.pro
make -f toolsMakefile

# Building Network Tools lib
createFolderIfNotPresent $NETWORKTOOLSLIB_BIN_PATH
cd $NETWORKTOOLSLIB_BIN_PATH
qmake -spec linux-arm-synology -o networktoolsMakefile "CONFIG+=synology" $NETWORKTOOLSLIB_PATH/NetworkToolsLib.pro
make -f networktoolsMakefile

# Building Parsers lib
createFolderIfNotPresent $PARSERSLIB_BIN_PATH1
createFolderIfNotPresent $PARSERSLIB_BIN_PATH
cd $PARSERSLIB_BIN_PATH
qmake -spec linux-arm-synology -o parsersMakefile "CONFIG+=synology" $PARSERSLIB_PATH/ParsersLib.pro
make -f parsersMakefile

# Building Task lib
createFolderIfNotPresent $TASKLIB_BIN_PATH 
cd $TASKLIB_BIN_PATH
qmake -spec linux-arm-synology -o taskMakefile "CONFIG+=synology" $TASKLIB_PATH/TaskLib.pro
make -f taskMakefile

# Building Task tool
createFolderIfNotPresent $TASKTOOL_BIN_PATH 
cd $TASKTOOL_BIN_PATH
qmake -spec linux-arm-synology -o execMakefile "CONFIG+=synology" $TASKTOOL_PATH/TaskTool.pro
make -f execMakefile

# Building Parser tool
createFolderIfNotPresent $PARSERTOOL_BIN_PATH
cd $PARSERTOOL_BIN_PATH
qmake -spec linux-arm-synology -o parserToolMakefile "CONFIG+=synology" $PARSERTOOL_PATH/CommandLineTool.pro
make -f parserToolMakefile

# Copy everything into synology folder, ready for transfer
if [ -d "$TARGET_DIRECTORY" ]; then
  rm -rf $TARGET_DIRECTORY
fi

mkdir $TARGET_DIRECTORY

cp $TOOLSLIB_BIN_PATH/libToolsLib.so $TARGET_DIRECTORY/
cp $NETWORKTOOLSLIB_BIN_PATH/libNetworkToolsLib.so $TARGET_DIRECTORY/
cp $PARSERSLIB_BIN_PATH/libParsersLib.so $TARGET_DIRECTORY/
cp $TASKLIB_BIN_PATH/libTaskLib.so $TARGET_DIRECTORY/
cp $TASKTOOL_BIN_PATH/TaskTool $TARGET_DIRECTORY/
cp $PARSERTOOL_BIN_PATH/CommandLineTool $TARGET_DIRECTORY/

