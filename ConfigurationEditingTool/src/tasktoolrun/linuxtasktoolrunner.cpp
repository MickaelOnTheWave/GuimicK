#include "linuxtasktoolrunner.h"

#include "tools.h"

void LinuxTaskToolRunner::Run()
{
   result = Tools::RunExternalCommandToBuffer(command, output, true);
   emit finished();
}
