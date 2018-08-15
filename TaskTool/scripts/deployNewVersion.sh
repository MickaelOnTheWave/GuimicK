
# This script makes the build using Desktop machine and copies the executable here.

TASKMANAGERROOT="~/Prog/TaskManager"
TOOLSROOT="~/Prog/Tools"
TASKTOOLPATH=$TASKMANAGERROOT"/TaskTool/"
BINPARTPATH="/bin/Synology"
TASKTOOLBIN=$TASKTOOLPATH$BINPARTPATH"/TaskTool"
PARSERBIN=$TASKMANAGERROOT"/ReportParsers/CommandLineTool"$BINPARTPATH"/CommandLineTool"
NETWORKTESTERBIN=$TOOLSROOT"/NetworkTester"$BINPARTPATH"/NetworkTester"

BINARIESPATH="~/Prog/TaskManager/TaskTool/bin/Synology"
CSSFILE=$TASKTOOLPATH"/data/report.css"
UNTESTED_TASKTOOL="untestedTaskTool"

LOCALLIBPATH="/usr/lib/"
TASKLIB="libTaskLib.so"
PARSERSLIB="libParsersLib.so"
NETWORKTOOLSLIB="libNetworkToolsLib.so"
TOOLSLIB="libToolsLib.so"
EXT=".1"

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

removeAllLibs()
{
	removeLib $TASKLIB
	removeLib $PARSERSLIB
	removeLib $NETWORKTOOLSLIB
	removeLib $TOOLSLIB
}

setupAllLibs()
{
	setupLib $TASKLIB
	setupLib $PARSERSLIB
	setupLib $NETWORKTOOLSLIB
	setupLib $TOOLSLIB
}

backupPreviousTool()
{
	if [ -f tasktool ]
	then
		mv tasktool tasktool.old
	fi
}

remoteBuild()
{
	ssh -t mickael@Desktop "cd $TASKTOOLPATH/scripts ; ./MakeAllSynologyBinaries.sh"
}

remoteCopyVersion()
{
	scp mickael@Desktop:$TASKTOOLBIN ./$UNTESTED_TASKTOOL
	mv $UNTESTED_TASKTOOL tasktool
	scp mickael@Desktop:$PARSERBIN ./reportparser
	scp mickael@Desktop:$NETWORKTESTERBIN ./networktester
	scp mickael@Desktop:$CSSFILE ./report.css
}

testVersion()
{
	if [ -f tasktool ]
	then
		echo "TaskTool compiled and copied."
		./tasktool --version
	else
		echo "TaskTool not available. Reverting to previous."
		mv tasktool.old tasktool
	fi
}

staticVersionDeploy()
{
	backupPreviousTool
	remoteBuild
	remoteCopyVersion
	testVersion
}

dynamicVersionDeploy()
{
	backupPreviousTool
	removeAllLibs
	remoteBuild
	remoteCopyVersion
	setupAllLibs
	testVersion
}


if [ "$1" = "dynamic" ]
then
	#Untested and obsolete. Needs fixing if used.
	dynamicVersionDeploy
else
	staticVersionDeploy
fi

