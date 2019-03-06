#include "sevenziptool.h"

#include "sevenzipcommandparser.h"
#include "tools.h"

using namespace std;

const string DefaultSevenZipCommand = "7z.exe";
const string SevenZipNotFoundString = "not recognized as an internal or external command";
const string error7zipNotFound = "7zip not found";

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

void SevenZipTool::CreateArchive(const string& pathData, ArchiveToolResult& result)
{
   AddToArchive(pathData, result);
}

void SevenZipTool::AddToArchive(const string& pathToAdd, ArchiveToolResult& result)
{
   const string command = sevenZipExecutable + " u -bb1 " + filename + " " + pathToAdd;
   Run7zipCommand(command, result);
}

void SevenZipTool::ExtractArchive(const string& destinationPath, ArchiveToolResult& result)
{
   const string command = sevenZipExecutable + " x " + filename + " -o" + destinationPath;
   Run7zipCommand(command, result);
}

void SevenZipTool::SetExecutablePath(const string& value)
{
   const string quote = "\"";
   sevenZipExecutable = quote + value + quote;
}

void SevenZipTool::Run7zipCommand(const string& command, ArchiveToolResult& result)
{
   string output;
   const int returnValue = Tools::RunExternalCommandToBuffer(command, output, true);
   ParseOutput(output, returnValue, result);
}

void SevenZipTool::ParseOutput(const string& output,
                               const int returnValue,
                               ArchiveToolResult& result) const
{
   result.isOk = (returnValue == 0);

   if (output.find(SevenZipNotFoundString) != string::npos)
      result.errorMessage = error7zipNotFound;
   else
   {
      SevenZipCommandParser parser;
      parser.ParseBuffer(output);
      ConvertToArchiveResult(parser, result);
   }
}
