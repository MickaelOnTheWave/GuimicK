#include "tartool.h"

#include "tarcommandparser.h"
#include "tools.h"

using namespace std;

const string tarCommand = "tar";
const string errorNoGzipUpdate = "Error : can't use Gzip compression";

TarTool::TarTool()
   : ArchiveTool(), useGzipCompression(true)
{
}

TarTool::TarTool(const TarTool& other)
   : ArchiveTool(),
     useGzipCompression(other.useGzipCompression)
{
}

ArchiveTool* TarTool::Clone() const
{
   return new TarTool(*this);
}

void TarTool::SetGzipCompression(const bool value)
{
   useGzipCompression = value;
}

void TarTool::CreateArchive(const string& pathData, ArchiveToolResult& result)
{
   const string command = tarCommand + " " + TarCreateFlags() + " " + filename + " " + pathData;
   RunTarCommand(command, result);
}

void TarTool::AddToArchive(const string& pathToAdd, ArchiveToolResult& result)
{
   const string command = tarCommand + " " + TarUpdateFlags() + " " + filename + " " + pathToAdd;
   RunTarCommand(command, result);
}

void TarTool::ExtractArchive(const string& destinationPath, ArchiveToolResult& result)
{
   const string command = tarCommand + " -xvf " + filename + " -C " + destinationPath;
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
   if (returnValue == 512)
      result.errorMessage = errorNoGzipUpdate;

   if (result.isOk)
   {
      TarCommandParser parser("tar");
      parser.ParseBuffer(output);
      ConvertToArchiveResult(parser, result);
   }
}

string TarTool::TarCreateFlags() const
{
   return (useGzipCompression) ? "-cpzvf" : "-cpvf";
}

string TarTool::TarUpdateFlags() const
{
   return (useGzipCompression) ? "-uzvf" : "-uvf";
}
