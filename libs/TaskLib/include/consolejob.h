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

   AbstractJob* Clone() override;

   bool IsInitialized(void) override;

   JobStatus* Run() override;

   static void Run(const std::wstring& command, const std::wstring& params);

   void RunWithoutStatus();

   int GetExpectedReturnCode() const override;
   void SetExpectedReturnCode(const int value) override;

   std::wstring GetCommand() const override;
   void SetCommand(const std::wstring& command) override;

   void SetCommandParameters(const std::wstring& parameters) override;
   std::wstring GetCommandParameters() const override;

   int GetCommandReturnCode() const override;
   void SetCommandReturnCode(const int value) override;

   std::wstring GetCommandOutput() const override;
   void SetCommandOutput(const std::wstring& value) override;

   bool IsRunOk() const override;

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

   void SetCommandWithAbsolutePath();

    std::wstring command;
    std::wstring commandParams;
    std::wstring commandOutput;
    int expectedReturnCode;
    int receivedReturnCode;
};

#endif // CONSOLEJOB_H
