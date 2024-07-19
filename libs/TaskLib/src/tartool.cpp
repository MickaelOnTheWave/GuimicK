#include "tartool.h"

#include "tarcommandparser.h"
#include "tools.h"

using namespace std;

const wstring tarCommand = L"tar";
const wstring errorNoGzipUpdate = L"Error : can't use Gzip compression";

const wstring space = L" ";

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

void TarTool::CreateArchive(const wstring& pathData, ArchiveToolResult& result)
{
   const wstring command = tarCommand + space + TarCreateFlags() + space + filename + space + pathData;
   RunTarCommand(command, result);
}

void TarTool::AddToArchive(const wstring& pathToAdd, ArchiveToolResult& result)
{
   const wstring command = tarCommand + space + TarUpdateFlags() + space + filename + space + pathToAdd;
   RunTarCommand(command, result);
}

void TarTool::ExtractArchive(const wstring& destinationPath, ArchiveToolResult& result)
{
   const wstring command = tarCommand + L" -xvf " + filename + L" -C " + destinationPath;
   RunTarCommand(command, result);
}

void TarTool::RunTarCommand(const wstring& command, ArchiveToolResult& result)
{
   wstring output;
   int returnValue = Tools::RunExternalCommandToBuffer(command, output, true);
   ParseOutput(output, returnValue, result);
}

void TarTool::ParseOutput(const wstring& output,
                          const int returnValue,
                          ArchiveToolResult& result) const
{
   result.isOk = (returnValue == 0);
   if (returnValue == 512)
      result.errorMessage = errorNoGzipUpdate;

   if (result.isOk)
   {
      TarCommandParser parser(L"tar");
      parser.ParseBuffer(output);
      ConvertToArchiveResult(parser, result);
   }
}

wstring TarTool::TarCreateFlags() const
{
   return (useGzipCompression) ? L"-cpzvf" : L"-cpvf";
}

wstring TarTool::TarUpdateFlags() const
{
   return (useGzipCompression) ? L"-uzvf" : L"-uvf";
}
