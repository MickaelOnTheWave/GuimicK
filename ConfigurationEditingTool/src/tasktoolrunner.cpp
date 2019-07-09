#include "tasktoolrunner.h"

#include "tools.h"

using namespace std;

void TaskToolRunner::Run()
{
   result = Tools::RunExternalCommandToBuffer(command, output, true);
   emit finished();
}

void TaskToolRunner::SetCommand(const wstring& value)
{
   command = value;
}

wstring TaskToolRunner::GetOutput() const
{
   return output;
}

int TaskToolRunner::GetReturnCode() const
{
   return result;
}
