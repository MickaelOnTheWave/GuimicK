#ifndef CONSOLEJOB_H
#define CONSOLEJOB_H

#include <string>

#include "AbstractConsoleJob.h"

/**
 * @brief Generic job ran from a console command.
 * It can run any console command, store its outputs and either
 * use outputs or return values to determine success or failure.
 *
 * @note A job cannot check an output file contents, thus it is impossible to
 * configure it to output to a file and have a success condition based on output content.
 */
class ConsoleJob : public AbstractConsoleJob
{
public:
    ConsoleJob(const std::string& _command = "",
               const std::string& _params = "", int _expectedReturnCode = 0);
    ConsoleJob(const ConsoleJob& other);
	virtual ~ConsoleJob();

	virtual std::string GetName();

    virtual AbstractJob* Clone();

    virtual bool InitializeFromClient(Client *);

    virtual bool IsInitialized(void);

	virtual JobStatus* Run();

    static void Run(const std::string& command, const std::string& params);

    void RunWithoutStatus();

    virtual int GetExpectedReturnCode() const;
    virtual void SetExpectedReturnCode(const int value);

    virtual std::string GetCommand() const;
    virtual void SetCommand(const std::string& command);

    virtual void SetCommandParameters(const std::string& parameters);
    std::string GetCommandParameters() const;

    virtual int GetCommandReturnCode();
    virtual std::string GetCommandOutput() const;


    // TODO : find a better place for that. This is a global feature, not
    // necessary related to this job.
    static void AddAppSearchPath(const std::string& path);
    static void ClearAppSearchPaths();
    static std::vector<std::string> appSearchPaths;

protected:

    virtual bool RunCommand();
    virtual JobStatus* CreateSuccessStatus();
    virtual JobStatus* CreateErrorStatus();

    std::string command;
    std::string commandParams;
    std::string commandOutput;
    int expectedReturnCode;
    int receivedReturnCode;
};

#endif // CONSOLEJOB_H
