
# This script makes the build using Desktop machine and copies the executable here.

TASKTOOLPATH="~/Prog/TaskManager/TaskTool/"
BINARIESPATH="~/Prog/TaskManager/bin/SynologyPackage"

ssh -t mickael@Desktop "cd $TASKTOOLPATH ; ./MakeSynologyBuild.sh"
scp mickael@Desktop:$BINARIESPATH/TaskTool ./tasktool
scp mickael@Desktop:$BINARIESPATH/CommandLineTool ./reportparser
scp mickael@Desktop:$BINARIESPATH/libTaskLib.so /usr/lib/libTaskLib.so
scp mickael@Desktop:$BINARIESPATH/libParsersLib.so /usr/lib/libParsersLib.so
scp mickael@Desktop:$BINARIESPATH/libToolsLib.so /usr/lib/libToolsLib.so

ln -s /usr/lib/libTaskLib.so /usr/lib/libTaskLib.so.1
ln -s /usr/lib/libParsersLib.so /usr/lib/libParsersLib.so.1
ln -s /usr/lib/libToolsLib.so /usr/lib/libToolsLib.so.1
