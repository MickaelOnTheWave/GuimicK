#include "abstracttasktoolrunner.h"

using namespace std;

void AbstractTaskToolRunner::SetCommand(const wstring& value)
{
   command = value;
}

wstring AbstractTaskToolRunner::GetOutput() const
{
   return output;
}

int AbstractTaskToolRunner::GetReturnCode() const
{
   return result;
}
