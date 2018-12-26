#include "tartool.h"

#include "tarcommandparser.h"
#include "tools.h"

using namespace std;

TarTool::TarTool(const string& _filename)
   : ArchiveTool(_filename), useGzipCompression(true)
{
}

void TarTool::SetGzipCompression(const bool value)
{
   useGzipCompression = value;
}

void TarTool::CreateArchive(const string& pathData, ArchiveToolResult& result)
{
   const string command = "tar " + TarCreateFlags() + " " + filename + " " + pathData;
   RunTarCommand(command, result);
}

void TarTool::AddToArchive(const string& pathToAdd, ArchiveToolResult& result)
{
   const string command = "tar " + TarUpdateFlags() + " " + filename + " " + pathToAdd;
   RunTarCommand(command, result);
}

void TarTool::ExtractArchive(const string& destinationPath, ArchiveToolResult& result)
{
   const string command = "tar -xvf " + filename + " -C " + destinationPath;
   RunTarCommand(command, result);
}

void TarTool::RunTarCommand(const string& command, ArchiveToolResult& result)
{
   string output;
   int returnValue = Tools::RunExternalCommandToBuffer(command, output, true);
   ParseOutput(output, returnValue, result);
}

void TarTool::ParseOutput(const string& output,
                          const int returnValue,
                          ArchiveToolResult& result) const
{
   result.isOk = (returnValue == 0);

   TarCommandParser parser("tar");
   parser.ParseBuffer(output);
   ConvertToArchiveResult(parser, result);
}

string TarTool::TarCreateFlags() const
{
   return (useGzipCompression) ? "-cpzvf" : "-cpvf";
}

string TarTool::TarUpdateFlags() const
{
   return (useGzipCompression) ? "-uzvf" : "-uvf";
}

void TarTool::ConvertToArchiveResult(TarCommandParser& parser,
                                     ArchiveToolResult& result) const
{
   FileBackupReport report;
   parser.GetReport(report);

   vector<string> addedFiles;
   report.GetAddedFiles(addedFiles);
   result.FileList = addedFiles;
}
