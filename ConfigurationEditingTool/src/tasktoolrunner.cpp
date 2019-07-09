#include "tasktoolrunner.h"

#include "tools.h"

using namespace std;

TaskToolRunner::TaskToolRunner(const wstring& _command)
   : command(_command)
{
}

void TaskToolRunner::Run()
{
   result = Tools::RunExternalCommandToBuffer(command, output, true);
   emit finished();
}

wstring TaskToolRunner::GetOutput() const
{
   return output;
}

int TaskToolRunner::GetReturnCode() const
{
   return result;
}
