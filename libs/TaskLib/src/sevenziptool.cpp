#include "sevenziptool.h"

#include "sevenzipcommandparser.h"
#include "tools.h"

using namespace std;

const wstring DefaultSevenZipCommand = L"7z.exe";
const wstring SevenZipNotFoundString = L"not recognized as an internal or external command";
const wstring moduleString = L"Can't load module";
const wstring dllString = L".dll";
const wstring error7zipNotFound = L"7zip not found";
const wstring error7zipDllNotFound = L"7zip Dll not found";
const wstring error7zipError = L"7zip execution error";

bool IsDllError(const wstring& output)
{
    return (output.find(moduleString) != wstring::npos &&
            output.find(dllString) != wstring::npos);
}

SevenZipTool::SevenZipTool()
   : ArchiveTool(), sevenZipExecutable(DefaultSevenZipCommand)
{
}

SevenZipTool::SevenZipTool(const SevenZipTool& other)
   : ArchiveTool(other),
     sevenZipExecutable(other.sevenZipExecutable)
{
}

ArchiveTool* SevenZipTool::Clone() const
{
   return new SevenZipTool(*this);
}

void SevenZipTool::CreateArchive(const wstring& pathData, ArchiveToolResult& result)
{
   AddToArchive(pathData, result);
}

void SevenZipTool::AddToArchive(const wstring& pathToAdd, ArchiveToolResult& result)
{
   const wstring command = sevenZipExecutable + L" u -bb1 " + filename + L" " + pathToAdd;
   Run7zipCommand(command, result);
}

void SevenZipTool::ExtractArchive(const wstring& destinationPath, ArchiveToolResult& result)
{
   const wstring command = sevenZipExecutable + L" x " + filename + L" -o" + destinationPath;
   Run7zipCommand(command, result);
}

void SevenZipTool::SetExecutablePath(const wstring& value)
{
   const wstring quote = L"\"";
   sevenZipExecutable = quote + value + quote;
}

void SevenZipTool::Run7zipCommand(const wstring& command, ArchiveToolResult& result)
{
   wstring output;
   const int returnValue = Tools::RunExternalCommandToBuffer(command, output, true);
   ParseOutput(output, returnValue, result);
}

#include <iostream>

void SevenZipTool::ParseOutput(const wstring& output,
                               const int returnValue,
                               ArchiveToolResult& result) const
{
   result.isOk = (returnValue == 0);

   if (result.isOk)
   {
      SevenZipCommandParser parser;
      parser.ParseBuffer(output);
      ConvertToArchiveResult(parser, result);
   }
   else
   {
       wcout << output << endl;
       if (output.find(SevenZipNotFoundString) != wstring::npos)
          result.errorMessage = error7zipNotFound;
       else if (returnValue == 2)
          result.errorMessage = error7zipDllNotFound;
       else {
           result.errorMessage = error7zipError;
       }

   }
}
