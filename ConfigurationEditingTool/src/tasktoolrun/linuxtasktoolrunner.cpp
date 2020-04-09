#include "linuxtasktoolrunner.h"

#include "errormessagecreator.h"
#include "tools.h"

ErrorMessageCreator* LinuxTaskToolRunner::CreateMessageCreator()
{
   return new ErrorMessageCreator();
}

void LinuxTaskToolRunner::RunFunction()
{
   runData.returnCode = Tools::RunExternalCommandToBuffer(command, runData.output, true);
   emit finished();
}
