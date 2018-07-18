#ifndef JOBEXECUTIONTARGET_H
#define JOBEXECUTIONTARGET_H

#include <string>

class JobExecutionTarget
{
public:
   JobExecutionTarget(const bool _isLocal = true) : isLocal(_isLocal) {}

   bool IsRemoteTargetConsistent() const
   {
       return (sshUser != "" && sshHost != "");
   }

   bool isLocal;
   std::string sshUser;
   std::string sshHost;
};

#endif // JOBEXECUTIONTARGET_H
