#ifndef DISKRELATEDJOB_H
#define DISKRELATEDJOB_H

#include "commandbasedjob.h"

class DiskRelatedJob : public CommandBasedJob
{
public:
   DiskRelatedJob(const std::wstring& jobName, const std::wstring& _drive);
   DiskRelatedJob(const DiskRelatedJob& other);

   std::wstring GetDrive() const;
   void SetDrive(const std::wstring& value);

protected:
   std::wstring drive;
};

#endif // DISKRELATEDJOB_H
