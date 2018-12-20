#include "tartool.h"

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

bool TarTool::CreateArchive(const string& pathData, ErrorList& errors)
{
   string output;
   const string command = "tar " + TarCreateFlags() + " " + filename + " " + pathData;
   int returnValue = Tools::RunExternalCommandToBuffer(command, output, true);
   return (returnValue == 0);
}

bool TarTool::AddToArchive(const string& pathToAdd, ErrorList& errors)
{
   string output;
   const string command = "tar " + TarUpdateFlags() + " " + filename + " " + pathToAdd;
   int returnValue = Tools::RunExternalCommandToBuffer(command, output, true);
   return (returnValue == 0);
}

bool TarTool::ExtractArchive(const string& destinationPath, ErrorList& errors)
{
   string output;
   const string command = "tar -xvf " + filename + " -C " + destinationPath;
   int returnValue = Tools::RunExternalCommandToBuffer(command, output, true);
   return (returnValue == 0);
}

string TarTool::TarCreateFlags() const
{
   return AddCompressionFlag("-cpvf");
}

string TarTool::TarUpdateFlags() const
{
   return AddCompressionFlag("-uvf");
}

string TarTool::AddCompressionFlag(const string& baseFlags) const
{
   return (useGzipCompression) ? baseFlags + "z" : baseFlags;
}
