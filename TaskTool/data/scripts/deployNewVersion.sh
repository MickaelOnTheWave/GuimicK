
# This script makes the build using Desktop machine and copies the executable here.

TASKTOOLPATH="~/Prog/TaskManager/TaskTool/"
BINARIESPATH="~/Prog/TaskManager/bin/SynologyPackage"

LOCALLIBPATH="/usr/lib/"
TASKLIB="libTaskLib.so"
PARSERSLIB="libParsersLib.so"
NETWORKTOOLSLIB="libNetworkToolsLib.so"
TOOLSLIB="libToolsLib.so"
CSSFILE=$TASKTOOLPATH"/data/report.css"

EXT=".1"
UNTESTED_TASKTOOL="untestedTaskTool"

STATIC_SETUP=true

if [ "$1" = "dynamic" ]
then
	STATIC_SETUP=false
fi

removeIfExists()
{
	FILE=$1
	if [ -f $FILE ]
	then
		rm $FILE
	fi
}

removeLib()
{
	LIB=$1
	removeIfExists $LOCALLIBPATH$LIB
	removeIfExists $LOCALLIBPATH$LIB$EXT
}

copyLibFromRemote()
{
	LIB=$1
	scp mickael@Desktop:$BINARIESPATH/$LIB $LOCALLIBPATH$LIB
}

createLibDynamicLink()
{
	LIB=$1
	ln -s $LOCALLIBPATH$LIB $LOCALLIBPATH$LIB$EXT
}

setupLib()
{
	LIB=$1
	copyLibFromRemote $LIB
	createLibDynamicLink $LIB
}

if [ -f tasktool ]
then
	mv tasktool tasktool.old
fi

if [ "$STATIC_SETUP" = false ]
then
	removeLib $TASKLIB
	removeLib $PARSERSLIB
	removeLib $NETWORKTOOLSLIB
	removeLib $TOOLSLIB
fi

ssh -t mickael@Desktop "cd $TASKTOOLPATH ; ./MakeSynologyBuild.sh"
scp mickael@Desktop:$BINARIESPATH/TaskTool ./$UNTESTED_TASKTOOL
mv $UNTESTED_TASKTOOL tasktool
scp mickael@Desktop:$BINARIESPATH/CommandLineTool ./reportparser
scp mickael@Desktop:$BINARIESPATH/NetworkTester ./networktester
scp mickael@Desktop:$CSSFILE ./report.css


if [ "$STATIC_SETUP" = false ]
then
	setupLib $TASKLIB
	setupLib $PARSERSLIB
	setupLib $NETWORKTOOLSLIB
	setupLib $TOOLSLIB
fi

if [ -f tasktool ]
then
	echo "TaskTool compiled and copied."
	./tasktool --version
else
	echo "TaskTool not available. Reverting to previous."
	mv tasktool.old tasktool
fi
