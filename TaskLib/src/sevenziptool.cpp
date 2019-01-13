#include "sevenziptool.h"

#include "tools.h"

using namespace std;

const string SevenZipCommand = "7z.exe";

SevenZipTool::SevenZipTool(const string& _filename)
   : ArchiveTool(_filename)
{
}

void SevenZipTool::CreateArchive(const string& pathData, ArchiveToolResult& result)
{
   AddToArchive(pathData, result);
}

void SevenZipTool::AddToArchive(const string& pathToAdd, ArchiveToolResult& result)
{
   const string command = SevenZipCommand + " a " + filename + " " + pathToAdd;
   Run7zipCommand(command, result);
}

void SevenZipTool::ExtractArchive(const string& destinationPath, ArchiveToolResult& result)
{
   const string command = SevenZipCommand + " x " + filename + " -o" + destinationPath;
   Run7zipCommand(command, result);
}

void SevenZipTool::Run7zipCommand(const string& command, ArchiveToolResult& result)
{
   string output;
   int returnValue = Tools::RunExternalCommandToBuffer(command, output, true);
   //ParseOutput(output, returnValue, result);
}
