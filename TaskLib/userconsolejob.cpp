#include "userconsolejob.h"

UserConsoleJob::UserConsoleJob()
{
}

UserConsoleJob::UserConsoleJob(const std::string &_commandTitle, const std::string &_commandName, const std::string &_commandParameters, int _expectedReturnCode)
{

}

UserConsoleJob::~UserConsoleJob()
{

}

JobStatus *UserConsoleJob::Run()
{
    JobStatus* status = ConsoleJob::Run();
    if (outputFileName == "")
        status->AddFileBuffer(commandTitle + ".txt", standardOutput);
}
