#ifndef ABSTRACTTASKTOOLRUNNER_H
#define ABSTRACTTASKTOOLRUNNER_H

#include <QObject>
#include <string>

#include "errormessagecreator.h"
#include "tasktooldata.h"

class AbstractTaskToolRunner : public QObject
{
   Q_OBJECT
public:
   virtual void Run() = 0;

   virtual ErrorMessageCreator* CreateMessageCreator() = 0;

   TaskToolRunData GetRunData() const;

   void SetCommand(const std::wstring& value);

signals:
   void finished();

protected:
   std::wstring command = L"";
   TaskToolRunData runData;
};

#endif // TASKTOOLRUNNER_H
