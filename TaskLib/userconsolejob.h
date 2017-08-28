#ifndef USERCONSOLEJOB_H
#define USERCONSOLEJOB_H

#include "consolejob.h"

#include "jobdebuginformationmanager.h"

class UserConsoleJob : public ConsoleJob
{
public:
    UserConsoleJob();
    UserConsoleJob(const std::string& _commandTitle,
                   const std::string& _command = "", const std::string& _params = "",
                   int _expectedReturnCode = 0);
    UserConsoleJob(const UserConsoleJob& other);
    virtual ~UserConsoleJob();

    virtual std::string GetName();

    virtual AbstractJob* Clone();

    virtual void Initialize(const std::string& _command,
                            int _expectedReturnCode = 0);

    void SetTitle(const std::string& title);

    std::string GetMiniDescriptionParserCommand() const;
    void SetMiniDescriptionParserCommand(const std::string& parser);

    void SetAttachOutput(const bool value);
    virtual void SetOutputDebugInformation(const bool value);

    std::string GetOutputFile() const;
    void SetOutputTofile(const std::string& filename);
    void SetOutputToBuffer(void);

    bool IsParsingUsingBuffer() const;
    void SetParsingUsingBuffer(const bool value);

    virtual void SetExpectedReturnCode(const int value);

    std::string GetExpectedOutput() const;
    void SetExpectedOutput(const std::string& value);

protected:
    virtual bool RunCommand();
    virtual JobStatus* CreateSuccessStatus();
    virtual JobStatus* CreateErrorStatus();

    void RunCommandOnFile();
    void RunCommandOnBuffer();
    bool IsRunOk();

    void FillStatusFromParsing();
    void FillErrorStatusFromOutput();
    void FillErrorStatusFromReturnCode();
    void FinalizeStatusCreation();

private:
    std::string CreateParserCommand() const;

    std::string commandTitle;
    std::string parserCommand;

    JobStatus* currentStatus;

    bool attachOutputToStatus;
    bool successConditionOnStandardOutput;
    bool useParserWithBuffer;
    std::string outputFileName;
    std::string expectedOutput;

    JobDebugInformationManager debugInfo;
};

#endif // USERCONSOLEJOB_H
