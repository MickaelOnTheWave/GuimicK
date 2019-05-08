#ifndef COMMANDBASEDJOB_H
#define COMMANDBASEDJOB_H

#include "abstractjob.h"

#include "AbstractConsoleJob.h"
#include "abstractoutputparser.h"

class CommandBasedJob : public AbstractJob
{
public:
   CommandBasedJob(const std::wstring& jobName);
   CommandBasedJob(const CommandBasedJob& other);

   virtual JobStatus* Run();

private:
   virtual AbstractConsoleJob* CreateJobInstance() const = 0;
   virtual AbstractOutputParser* CreateOutputParser() const = 0;

   void AddJobOutputToStatus(
           JobStatus* status,
           AbstractConsoleJob* job
        );
   void AddParsedOutputToStatus(
           JobStatus* status,
           AbstractOutputParser* parser,
           AbstractConsoleJob* job
        );
   void AddRawOutputToStatus(
           JobStatus* status,
           AbstractConsoleJob* job
        );
};
#endif // COMMANDBASEDJOB_H
