
# This script makes the build using Desktop machine and copies the executable here.

TASKTOOLPATH="~/Prog/TaskManager/TaskTool/"
BINARIESPATH="~/Prog/TaskManager/bin/SynologyPackage"

LOCALTASKLIB="/usr/lib/libTaskLib.so"
LOCALPARSERSLIB="/usr/lib/libParsersLib.so"
LOCALTOOLSLIB="/usrÂ/lib/libToolsLib.so"

EXT=".1"

mv tasktool tasktool.old
rm $LOCALTASKLIB
rm $LOCALTASKLIB$EXT
rm $LOCALPARSERSLIB
rm $LOCALPARSERSLIB$EXT
rm $LOCALTOOLSLIB
rm $LOCALTOOLSLIB$EXT

ssh -t mickael@Desktop "cd $TASKTOOLPATH ; ./MakeSynologyBuild.sh"
scp mickael@Desktop:$BINARIESPATH/TaskTool ./lalala
mv lalala tasktool
scp mickael@Desktop:$BINARIESPATH/CommandLineTool ./reportparser
scp mickael@Desktop:$BINARIESPATH/libTaskLib.so $LOCALTASKLIB
scp mickael@Desktop:$BINARIESPATH/libParsersLib.so $LOCALPARSERSLIB
scp mickael@Desktop:$BINARIESPATH/libToolsLib.so $LOCALTOOLSLIB

ln -s $LOCALTASKLIB $LOCALTASKLIB$EXT
ln -s $LOCALPARSERSLIB $LOCALPARSERSLIB$EXT
ln -s $LOCALTOOLSLIB $LOCALTOOLSLIB$EXT

if [ -f tasktool ]
then
	echo "TaskTool compiled and copied."
	./tasktool --version
else
	echo "TaskTool not available. Reverting to previous."
	mv tasktool.old tasktool
fi
