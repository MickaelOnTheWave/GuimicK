#include "linuxtasktoolrunner.h"

#include "errormessagecreator.h"
#include "tools.h"

void LinuxTaskToolRunner::Run()
{
   runData.returnCode = Tools::RunExternalCommandToBuffer(command, runData.output, true);
   emit finished();
}

ErrorMessageCreator* LinuxTaskToolRunner::CreateMessageCreator()
{
   return new ErrorMessageCreator();
}
