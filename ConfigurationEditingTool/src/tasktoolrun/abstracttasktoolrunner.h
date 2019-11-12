#ifndef ABSTRACTTASKTOOLRUNNER_H
#define ABSTRACTTASKTOOLRUNNER_H

#include <QObject>
#include <string>

class AbstractTaskToolRunner : public QObject
{
   Q_OBJECT
public:
   virtual void Run() = 0;

   void SetCommand(const std::wstring& value);

   std::wstring GetOutput() const;
   int GetReturnCode() const;

signals:
   void finished();

protected:
   std::wstring command = L"";
   std::wstring output = L"";
   int result = -1;
};

#endif // TASKTOOLRUNNER_H
