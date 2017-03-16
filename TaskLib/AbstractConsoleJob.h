#ifndef ABSTRACTCONSOLEJOB_H
#define ABSTRACTCONSOLEJOB_H

#include "abstractjob.h"

class AbstractConsoleJob : public AbstractJob
{
public:
    virtual ~AbstractConsoleJob() {}

    virtual int GetExpectedReturnCode() const = 0;
    virtual void SetExpectedReturnCode(const int value) = 0;

    virtual std::string GetCommand() const = 0;
    virtual void SetCommand(const std::string& command) = 0;

    virtual int GetCommandReturnCode() = 0;
    virtual std::string GetCommandOutput() const = 0;

};

#endif // ABSTRACTCONSOLEJOB_H
