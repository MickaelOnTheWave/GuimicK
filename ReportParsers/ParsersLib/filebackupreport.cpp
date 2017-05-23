#include "filebackupreport.h"

#include <algorithm>
#include <sstream>

using namespace std;

FileBackupReport::FileBackupReport()
{
}

FileBackupReport::~FileBackupReport()
{
}

void FileBackupReport::Clear()
{
    added.clear();
    modified.clear();
    removed.clear();
}

void FileBackupReport::operator=(const FileBackupReport &other)
{
    added.clear();
    std::copy(other.added.begin(), other.added.end(), std::back_inserter(added));

    removed.clear();
    std::copy(other.removed.begin(), other.removed.end(), std::back_inserter(removed));

    modified.clear();
    std::copy(other.modified.begin(), other.modified.end(), std::back_inserter(modified));
}

void FileBackupReport::SortData()
{
    std::sort(added.begin(), added.end());
    std::sort(removed.begin(), removed.end());
    std::sort(modified.begin(), modified.end());
}

std::string FileBackupReport::GetMiniDescription() const
{
    stringstream description;
    description << added.size() << " added, ";
    description << modified.size() << " modified, ";
    description << removed.size() << " removed";
    return description.str();
}

std::string FileBackupReport::GetFullDescription() const
{
    stringstream description;
    description << FileListDescription(added, "added");
    description << FileListDescription(modified, "modified");
    description << FileListDescription(removed, "removed");
    return description.str();
}

void FileBackupReport::AddAsAdded(const std::vector<string> &files)
{
    copy(files.begin(), files.end(), back_inserter(added));
}

void FileBackupReport::AddAsAdded(const string &file)
{
    added.push_back(file);
}

void FileBackupReport::AddAsModified(const string &file)
{
    modified.push_back(file);
}

void FileBackupReport::AddAsRemoved(const string &file)
{
    removed.push_back(file);
}

void FileBackupReport::Add(const FileBackupReport &otherReport)
{
    copy(otherReport.added.begin(), otherReport.added.end(), back_inserter(added));
    copy(otherReport.modified.begin(), otherReport.modified.end(), back_inserter(modified));
    copy(otherReport.removed.begin(), otherReport.removed.end(), back_inserter(removed));
}

void FileBackupReport::AddWithPrefix(const FileBackupReport &otherReport, const string &prefix)
{
    CopyPrefixed(added, otherReport.added, prefix);
    CopyPrefixed(modified, otherReport.modified, prefix);
    CopyPrefixed(removed, otherReport.removed, prefix);
}

string FileBackupReport::FileListDescription(const std::vector<string> &fileList,
                                             const string &operation) const
{
    stringstream description;
    description << fileList.size() << " files " << operation << std::endl;

    vector<string>::const_iterator it=fileList.begin();
    vector<string>::const_iterator end=fileList.end();
    for (; it!=end; it++)
        description << "\t" << *it << std::endl;
    description << std::endl;

    return description.str();
}

void FileBackupReport::CopyPrefixed(std::vector<string> &destination,
                                    const std::vector<string> &source, const string &prefix)
{
    vector<string> prefixedSource(source);
    vector<string>::iterator it = prefixedSource.begin();
    for (; it!=prefixedSource.end(); ++it)
        *it = prefix + *it;

    copy(prefixedSource.begin(), prefixedSource.end(), back_inserter(destination));
}
