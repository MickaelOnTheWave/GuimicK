#include "filebackupreport.h"

#include <algorithm>
#include <sstream>

using namespace std;

FileBackupReport::FileBackupReport()
{
}

FileBackupReport::FileBackupReport(const FileBackupReport& other)
   : added(other.added),
     modified(other.modified),
     removed(other.removed)
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

FileBackupReport& FileBackupReport::operator=(const FileBackupReport &other)
{
    added.clear();
    std::copy(other.added.begin(), other.added.end(), std::back_inserter(added));

    removed.clear();
    std::copy(other.removed.begin(), other.removed.end(), std::back_inserter(removed));

    modified.clear();
    std::copy(other.modified.begin(), other.modified.end(), std::back_inserter(modified));

    return *this;
}

void FileBackupReport::SortData()
{
    std::sort(added.begin(), added.end());
    std::sort(removed.begin(), removed.end());
    std::sort(modified.begin(), modified.end());
}

std::wstring FileBackupReport::GetMiniDescription() const
{
    wstringstream description;
    description << added.size() << " added, ";
    description << modified.size() << " modified, ";
    description << removed.size() << " removed";
    return description.str();
}

std::wstring FileBackupReport::GetFullDescription() const
{
    wstringstream description;
    description << FileListDescription(added, L"added");
    description << FileListDescription(modified, L"modified");
    description << FileListDescription(removed, L"removed");
    return description.str();
}

void FileBackupReport::AddAsAdded(const std::vector<wstring> &files)
{
    copy(files.begin(), files.end(), back_inserter(added));
}

void FileBackupReport::AddAsAdded(const wstring &file)
{
   added.push_back(file);
}

void FileBackupReport::AddAsModified(const std::vector<wstring>& files)
{
   copy(files.begin(), files.end(), back_inserter(modified));
}

void FileBackupReport::AddAsModified(const wstring &file)
{
   modified.push_back(file);
}

void FileBackupReport::AddAsRemoved(const std::vector<wstring>& files)
{
   copy(files.begin(), files.end(), back_inserter(removed));
}

void FileBackupReport::AddAsRemoved(const wstring &file)
{
    removed.push_back(file);
}

void FileBackupReport::Add(const FileBackupReport &otherReport)
{
    copy(otherReport.added.begin(), otherReport.added.end(), back_inserter(added));
    copy(otherReport.modified.begin(), otherReport.modified.end(), back_inserter(modified));
    copy(otherReport.removed.begin(), otherReport.removed.end(), back_inserter(removed));
}

void FileBackupReport::AddWithPrefix(const FileBackupReport &otherReport, const wstring &prefix)
{
    CopyPrefixed(added, otherReport.added, prefix);
    CopyPrefixed(modified, otherReport.modified, prefix);
    CopyPrefixed(removed, otherReport.removed, prefix);
}

void FileBackupReport::GetAddedFiles(std::vector<wstring>& files)
{
   files = added;
}

void FileBackupReport::GetModifiedFiles(std::vector<wstring>& files)
{
   files = modified;
}

void FileBackupReport::GetRemovedFiles(std::vector<wstring>& files)
{
   files = removed;
}

wstring FileBackupReport::FileListDescription(const std::vector<wstring> &fileList,
                                             const wstring &operation) const
{
    wstringstream description;
    description << fileList.size() << " files " << operation << std::endl;

    vector<wstring>::const_iterator it=fileList.begin();
    vector<wstring>::const_iterator end=fileList.end();
    for (; it!=end; it++)
        description << "\t" << *it << std::endl;
    description << std::endl;

    return description.str();
}

void FileBackupReport::CopyPrefixed(std::vector<wstring> &destination,
                                    const std::vector<wstring> &source, const wstring &prefix)
{
    vector<wstring> prefixedSource(source);
    vector<wstring>::iterator it = prefixedSource.begin();
    for (; it!=prefixedSource.end(); ++it)
        *it = prefix + *it;

    copy(prefixedSource.begin(), prefixedSource.end(), back_inserter(destination));
}
