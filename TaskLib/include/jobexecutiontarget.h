#ifndef JOBEXECUTIONTARGET_H
#define JOBEXECUTIONTARGET_H

#include <string>

class JobExecutionTarget
{
public:
   JobExecutionTarget(const bool _isLocal = true) : isLocal(_isLocal) {}

   bool IsRemoteTargetConsistent() const
   {
       return (sshUser != L"" && sshHost != L"");
   }

   bool isLocal;
   std::wstring sshUser;
   std::wstring sshHost;
};

#endif // JOBEXECUTIONTARGET_H
