#ifndef CONSOLEWAKEJOB_H
#define CONSOLEWAKEJOB_H

#include "abstractwakejob.h"
#include "consolejob.h"

class ConsoleWakeJob : public AbstractWakeJob
{
public:
    ConsoleWakeJob();
    ConsoleWakeJob(const ConsoleWakeJob& other);
    virtual ~ConsoleWakeJob();

    virtual AbstractJob* Clone();

private:
    virtual JobStatus* SetupWaker();
    virtual JobStatus* RunWaker();

    ConsoleJob wakeCommand;
};

#endif // WAKEJOB_H
