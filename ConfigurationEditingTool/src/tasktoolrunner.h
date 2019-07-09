#ifndef TASKTOOLRUNNER_H
#define TASKTOOLRUNNER_H

#include <QObject>
#include <string>

class TaskToolRunner : public QObject
{
   Q_OBJECT
public:
   void Run();

   void SetCommand(const std::wstring& value);

   std::wstring GetOutput() const;
   int GetReturnCode() const;

signals:
   void finished();

private:
   std::wstring command = L"";
   std::wstring output = L"";
   int result = -1;
};

#endif // TASKTOOLRUNNER_H
