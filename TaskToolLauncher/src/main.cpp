#include <sstream>

#include "filetools.h"
#include "tools.h"

using namespace std;

const wstring returnCodeFile = L"taskToolReturnCode";
const std::wstring outputFile = L"taskToolOutput";
const std::wstring commandFile = L"taskToolCommand";

std::wstring ToWstring(const int value)
{
   wstringstream stream;
   stream << value;
   return stream.str();
}

int main(void)
{
   const std::wstring commandToRun = FileTools::GetTextFileContent(commandFile);
   const int returnCode = Tools::RunExternalCommandToFile(commandToRun, outputFile);
   FileTools::WriteBufferToFile(returnCodeFile, ToWstring(returnCode));
   return 0;
}
