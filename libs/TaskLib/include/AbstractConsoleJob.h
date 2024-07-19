#ifndef ABSTRACTCONSOLEJOB_H
#define ABSTRACTCONSOLEJOB_H

#include "abstractjob.h"

class AbstractConsoleJob : public AbstractJob
{
public:
   AbstractConsoleJob(const std::wstring& _title);
   virtual ~AbstractConsoleJob();

   virtual int GetExpectedReturnCode() const = 0;
   virtual void SetExpectedReturnCode(const int value) = 0;

   virtual std::wstring GetCommand() const = 0;
   virtual void SetCommand(const std::wstring& command) = 0;

   virtual std::wstring GetCommandParameters() const = 0;
   virtual void SetCommandParameters(const std::wstring& parameters) = 0;

   virtual int GetCommandReturnCode() const = 0;
   virtual void SetCommandReturnCode(const int value) = 0;

   virtual std::wstring GetCommandOutput() const = 0;
   virtual void SetCommandOutput(const std::wstring& value) = 0;

   virtual bool IsRunOk() const = 0;
};

#endif // ABSTRACTCONSOLEJOB_H
