
# This script makes the build using Desktop machine and copies the executable here.

TASKTOOLPATH="~/Prog/TaskManager/TaskTool/"
BINARIESPATH="~/Prog/TaskManager/bin/SynologyPackage"

LOCALLIBPATH="/usr/lib/"
TASKLIB="libTaskLib.so"
PARSERSLIB="libParsersLib.so"
TOOLSLIB="libToolsLib.so"
CSSFILE=$TASKTOOLPATH"/data/report.css"

EXT=".1"
UNTESTED_TASKTOOL="untestedTaskTool"

removeIfExists()
{
	FILE=$1
	if [ -f $FILE ]
	then
		rm $FILE
	fi
}


if [ -f tasktool ]
then
	mv tasktool tasktool.old
fi

removeIfExists $LOCALLIBPATH$TASKLIB
removeIfExists $LOCALLIBPATH$TASKLIB$EXT
removeIfExists $LOCALLIBPATH$PARSERSLIB
removeIfExists $LOCALLIBPATH$PARSERSLIB$EXT
removeIfExists $LOCALLIBPATH$TOOLSLIB
removeIfExists $LOCALLIBPATH$TOOLSLIB$EXT

ssh -t mickael@Desktop "cd $TASKTOOLPATH ; ./MakeSynologyBuild.sh"
scp mickael@Desktop:$BINARIESPATH/TaskTool ./$UNTESTED_TASKTOOL
mv $UNTESTED_TASKTOOL tasktool
scp mickael@Desktop:$BINARIESPATH/CommandLineTool ./reportparser
scp mickael@Desktop:$CSSFILE ./report.css
scp mickael@Desktop:$BINARIESPATH/$TASKLIB $LOCALLIBPATH$TASKLIB
scp mickael@Desktop:$BINARIESPATH/$PARSERSLIB $LOCALLIBPATH$PARSERSLIB
scp mickael@Desktop:$BINARIESPATH/$TOOLSLIB $LOCALLIBPATH$TOOLSLIB

ln -s $LOCALLIBPATH$TASKLIB $LOCALLIBPATH$TASKLIB$EXT
ln -s $LOCALLIBPATH$PARSERSLIB $LOCALLIBPATH$PARSERSLIB$EXT
ln -s $LOCALLIBPATH$TOOLSLIB $LOCALLIBPATH$TOOLSLIB$EXT

if [ -f tasktool ]
then
	echo "TaskTool compiled and copied."
	./tasktool --version
else
	echo "TaskTool not available. Reverting to previous."
	mv tasktool.old tasktool
fi
