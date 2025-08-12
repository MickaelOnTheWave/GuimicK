#ifndef USERCONSOLEJOB_H
#define USERCONSOLEJOB_H

#include "consolejob.h"

#include "jobdebuginformationmanager.h"

class UserConsoleJob : public ConsoleJob
{
public:
    UserConsoleJob();
    UserConsoleJob(const std::wstring& _commandTitle,
                   const std::wstring& _command = L"", const std::wstring& _params = L"",
                   int _expectedReturnCode = 0);
    UserConsoleJob(const UserConsoleJob& other);
    virtual ~UserConsoleJob();

    AbstractJob* Clone() override;

    virtual void Initialize(const std::wstring& _command,
                            int _expectedReturnCode = 0);

    JobStatus* Run() override;

    std::wstring GetMiniDescriptionParserCommand() const;
    void SetMiniDescriptionParserCommand(const std::wstring& parser);

    void SetAttachOutput(const bool value);

    std::wstring GetOutputFile() const;
    void SetOutputTofile(const std::wstring& filename);
    void SetOutputToBuffer(void);

    bool IsParsingUsingBuffer() const;
    void SetParsingUsingBuffer(const bool value);

    void SetExpectedReturnCode(const int value) override;

    bool IsRunOk() const override;

    std::wstring GetExpectedOutput() const;
    void SetExpectedOutput(const std::wstring& value);

    bool HasUserAttachments() const;
    void GetUserAttachments(std::vector<std::wstring>& attachments);
    void AddUserAttachment(const std::wstring& name);
    void EmptyUserAttachments();

protected:
    bool RunCommand() override;
    JobStatus* CreateSuccessStatus() override;
    JobStatus* CreateErrorStatus() override;

    void RunCommandOnFile();
    void RunCommandOnBuffer();

    void FillStatusFromParsing();
    void FillErrorStatusFromOutput();
    void FillErrorStatusFromReturnCode();
    void FinalizeStatusCreation();

private:
    std::wstring CreateParserCommand() const;

    std::wstring parserCommand;

    JobStatus* currentStatus;

    bool attachOutputToStatus;
    bool successConditionOnStandardOutput;
    bool useParserWithBuffer;
    std::wstring outputFileName;
    std::wstring expectedOutput;
    std::vector<std::wstring> additionalAttachments;
};

#endif // USERCONSOLEJOB_H
