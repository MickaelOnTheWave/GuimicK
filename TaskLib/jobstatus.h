#ifndef JOBSTATUS_H
#define JOBSTATUS_H

#include <ctime>
#include <string>
#include <vector>

class JobStatus
{
public:
    static const int NOT_EXECUTED;
    static const int OK;
    static const int OK_WITH_WARNINGS;
    static const int ERROR;

	JobStatus();
	JobStatus(int _code, const std::string& _description = "");
    JobStatus(const JobStatus& other);
	~JobStatus();

	void Reset();

	int GetCode() const;
	void SetCode(int _code);

    static std::string GetCodeDescription(int _code);
    static int GetCodeFromDescription(const std::string& _description);
    std::string GetCodeDescription() const;

	std::time_t GetDuration() const;
	void SetDuration(std::time_t duration);

	std::string GetDescription() const;
	void SetDescription(const std::string& _description);

    void AddExternalFile(const std::string& filename);
    void AddFileBuffer(const std::string& filename, const std::string& filecontents);
    void AddExternalFilesFromStatus(JobStatus* other);
    void AddFileBuffersFromStatus(JobStatus* other);
    void AddAllFilesFromStatus(JobStatus* other);
    void ClearAllFiles();

	bool IsWorseThan(int otherCode) const;

	/**
		A status is considered fatal if no processing can be done based
		on its execution (ERROR or NOT_EXECUTED for example).
	  */
	bool IsFatal() const;

	std::string ToString() const;


    bool HasExternalFiles();

    typedef std::vector<std::pair<std::string,std::string> > FileBufferList;
    void GetExternalFilenames(std::vector<std::string>& _fileNames);
    void GetFileBuffers(FileBufferList& _filebuffers);

protected:
	int			code;
	std::time_t elapsedTime;
	std::string description;

    std::vector<std::string> externalFilenames;
    std::vector<std::pair<std::string, std::string> > filebuffers;
};

#endif // JOBSTATUS_H
