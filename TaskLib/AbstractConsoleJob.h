#ifndef ABSTRACTCONSOLEJOB_H
#define ABSTRACTCONSOLEJOB_H

#include "abstractjob.h"

class AbstractConsoleJob : public AbstractJob
{
public:
   AbstractConsoleJob(const std::string& _title);
   virtual ~AbstractConsoleJob();

   virtual int GetExpectedReturnCode() const = 0;
   virtual void SetExpectedReturnCode(const int value) = 0;

   virtual std::string GetCommand() const = 0;
   virtual void SetCommand(const std::string& command) = 0;

   virtual std::string GetCommandParameters() const = 0;
   virtual void SetCommandParameters(const std::string& parameters) = 0;

   virtual int GetCommandReturnCode() const = 0;
   virtual void SetCommandReturnCode(const int value) = 0;

   virtual std::string GetCommandOutput() const = 0;
   virtual void SetCommandOutput(const std::string& value) = 0;

   virtual bool IsRunOk() const = 0;
};

#endif // ABSTRACTCONSOLEJOB_H
