#include "archivetool.h"
#include "tools.h"

#include <algorithm>

using namespace std;

ArchiveTool::ArchiveTool() : filename("")
{
}

ArchiveTool::ArchiveTool(const ArchiveTool& other)
   : filename(other.filename)
{
}

ArchiveTool::~ArchiveTool()
{
}

void ArchiveTool::Initialize(const std::string& _filename)
{
   filename = _filename;
}

void ArchiveTool::ConvertToArchiveResult(AbstractFileBackupParser& parser,
                                         ArchiveToolResult& result) const
{
   parser.GetReport(result.FileList);
}