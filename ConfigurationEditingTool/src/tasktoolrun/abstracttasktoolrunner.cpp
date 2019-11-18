#include "abstracttasktoolrunner.h"

using namespace std;

TaskToolRunData AbstractTaskToolRunner::GetRunData() const
{
   return runData;
}

void AbstractTaskToolRunner::SetCommand(const wstring& value)
{
   command = value;
}
