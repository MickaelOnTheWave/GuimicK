#include "logicaldrive.h"

using namespace std;

LogicalDrive::LogicalDrive()
   : name(L""), totalSpace(L""), usedSpace(L""), freeSpace(L""), ratio(L""),
     badSectors(L""), hasBadSectors(false)
{
}

LogicalDrive::LogicalDrive(const wstring& _name, const wstring& _totalSpace,
                           const wstring& _usedSpace, const wstring& _freeSpace,
                           const wstring& _ratio, const wstring& _badSectors)
   : name(_name), totalSpace(_totalSpace), usedSpace(_usedSpace),
     freeSpace(_freeSpace), ratio(_ratio), badSectors(_badSectors),
     hasBadSectors(false)
{
}

bool LogicalDrive::HasBadSectors() const
{
   return hasBadSectors;
}
