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
   static std::wstring NotAvailableError;

   ConsoleJob(const std::wstring& _command = L"",
            const std::wstring& _params = L"", int _expectedReturnCode = 0);
   ConsoleJob(const ConsoleJob& other);
   virtual ~ConsoleJob();

   virtual AbstractJob* Clone();

   virtual bool IsInitialized(void);

   virtual JobStatus* Run();

   static void Run(const std::wstring& command, const std::wstring& params);

   void RunWithoutStatus();

   virtual int GetExpectedReturnCode() const;
   virtual void SetExpectedReturnCode(const int value);

   virtual std::wstring GetCommand() const;
   virtual void SetCommand(const std::wstring& command);

   virtual void SetCommandParameters(const std::wstring& parameters);
   std::wstring GetCommandParameters() const;

   virtual int GetCommandReturnCode() const;
   virtual void SetCommandReturnCode(const int value);

   virtual std::wstring GetCommandOutput() const;
   virtual void SetCommandOutput(const std::wstring& value);

   virtual bool IsRunOk() const;

   virtual bool IsCommandAvailable() const;


   // TODO : find a better place for that. This is a global feature, not
   // necessary related to this job.
   static void AddAppSearchPath(const std::wstring& path);
   static void ClearAppSearchPaths();
   static std::vector<std::wstring> appSearchPaths;

protected:

    virtual bool RunCommand();

    virtual JobStatus* CreateSuccessStatus();
    virtual JobStatus* CreateErrorStatus();

    std::wstring command;
    std::wstring commandParams;
    std::wstring commandOutput;
    int expectedReturnCode;
    int receivedReturnCode;
};

#endif // CONSOLEJOB_H
