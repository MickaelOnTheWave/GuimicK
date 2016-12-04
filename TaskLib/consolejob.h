#ifndef CONSOLEJOB_H
#define CONSOLEJOB_H

#include <string>

#include "abstractjob.h"

/**
 * @brief Generic job ran from a console command.
 * It can run any console command, store its outputs and either
 * use outputs or return values to determine success or failure.
 *
 * @note A job cannot check an output file contents, thus it is impossible to
 * configure it to output to a file and have a success condition based on output content.
 */
class ConsoleJob : public AbstractJob
{
public:
    ConsoleJob();
    ConsoleJob(const std::string& _commandTitle, const std::string& _commandName = "",
               const std::string& _commandParameters = "", int _expectedReturnCode = 0);
	virtual ~ConsoleJob();

	virtual std::string GetName();

    virtual AbstractJob* Clone();

    virtual bool InitializeFromClient(Client *);

    void SetTitle(const std::string& _title);

    void SetMiniDescriptionParserCommand(const std::string& parser);

    virtual void Initialize(const std::string& _commandName,
                            const std::string& _commandParameters = "", int _expectedReturnCode = 0);

    virtual bool IsInitialized(void);

	virtual JobStatus* Run();

	int GetCommandReturnCode();

    /**
     * @return Output content if command was configured to run using string buffer,
     * and output file name if it was configured to use an external file to store output.
     */
    std::string GetCommandOutput() const;

    void SetOutputTofile(const std::string& filename);
    void SetOutputToBuffer(void);

    void EnableSuccessOnReturnCode(int code);
    void DisableSuccessOnReturnCode();
    void EnableSuccessOnOutput(const std::string& output);
    void DisableSuccessOnOutput();

    static void AddAppSearchPath(const std::string& path);
    static void ClearAppSearchPaths();

protected:

    virtual std::string CreateFullCommand(void);
    /**
        This is used only for having a beautiful name on reports
    */
    std::string commandTitle;

	std::string commandName;
    std::string commandParameters;
    std::string parserCommand;

    bool checkReturnCode;
    bool checkStandardOutput;

    std::string outputFileName;
    std::string standardOutput;
    std::string expectedOutput;

	int expectedReturnCode;
	int receivedReturnCode;

    static std::vector<std::string> appSearchPaths;
};

#endif // CONSOLEJOB_H
