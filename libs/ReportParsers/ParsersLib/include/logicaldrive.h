#ifndef LOGICALDRIVE_H
#define LOGICALDRIVE_H

#include <string>

class LogicalDrive
{
public :
    LogicalDrive();
    LogicalDrive(const std::wstring& _name,
                 const std::wstring& _totalSpace,
                 const std::wstring& _usedSpace,
                 const std::wstring& _freeSpace,
                 const std::wstring& _ratio,
                 const std::wstring& _badSectors);

    bool HasBadSectors() const;

    std::wstring name;
    std::wstring totalSpace;
    std::wstring usedSpace;
    std::wstring freeSpace;
    std::wstring ratio;
    std::wstring badSectors;
    bool hasBadSectors;
};

#endif // LOGICALDRIVE_H
