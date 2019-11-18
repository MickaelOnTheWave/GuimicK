#ifndef WINDOWSTASKTOOLRUNNER_H
#define WINDOWSTASKTOOLRUNNER_H

#include "abstracttasktoolrunner.h"

class WindowsTaskToolRunner : public AbstractTaskToolRunner
{
    Q_OBJECT
public:
   void Run() override;

   ErrorMessageCreator* CreateMessageCreator() override;

private:
   bool ExecuteTaskToolAsAdmin();
   void WaitUntilExecutionIsComplete();
   void SetErrorMessage();

   unsigned long shellExecuteErrorCode;
};

#endif // WINDOWSTASKTOOLRUNNER_H
