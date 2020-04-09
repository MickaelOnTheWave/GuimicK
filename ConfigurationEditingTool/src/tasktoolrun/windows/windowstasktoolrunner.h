#ifndef WINDOWSTASKTOOLRUNNER_H
#define WINDOWSTASKTOOLRUNNER_H

#include "abstracttasktoolrunner.h"

class WindowsTaskToolRunner : public AbstractTaskToolRunner
{
    Q_OBJECT
public:
   WindowsTaskToolRunner();

   ErrorMessageCreator* CreateMessageCreator() override;

private:
   void RunFunction() override;

   bool ExecuteTaskToolAsAdmin();
   void WaitUntilExecutionIsComplete();

   void PrepareTaskToolLaunch();

   void CreateTaskToolLauncherFullPath();

   static std::wstring GetLauncherExecutablePath();

   int GetTaskToolReturnCode() const;
   std::wstring GetTaskToolOutput() const;

   unsigned long shellExecuteErrorCode;
   std::string launcherFullPath;
   const std::wstring returnCodeFile = L"taskToolReturnCode";
};

#endif // WINDOWSTASKTOOLRUNNER_H
