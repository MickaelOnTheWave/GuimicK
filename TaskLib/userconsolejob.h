#ifndef USERCONSOLEJOB_H
#define USERCONSOLEJOB_H

#include "consolejob.h"

class UserConsoleJob : public ConsoleJob
{
public:
    UserConsoleJob();
    UserConsoleJob(const std::string& _commandTitle, const std::string& _commandName = "",
               const std::string& _commandParameters = "", int _expectedReturnCode = 0);
    virtual ~UserConsoleJob();

    virtual JobStatus* Run();

};

#endif // USERCONSOLEJOB_H
