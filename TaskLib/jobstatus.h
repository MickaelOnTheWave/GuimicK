#ifndef JOBSTATUS_H
#define JOBSTATUS_H

#include <ctime>
#include <string>
#include <vector>

class JobStatus
{
public:
	static const int NOT_EXECUTED			= 0;
	static const int OK						= 1;
	static const int OK_WITH_WARNINGS	= 2;
	static const int ERROR					= 3;

	JobStatus();
	JobStatus(int _code, const std::string& _description = "");
	~JobStatus();

	void Reset();

	int GetCode() const;
	void SetCode(int _code);

    static std::string GetCodeDescription(int _code);
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

	/**
		@param[out] fileNames
	*/
    void GetExternalFilenames(std::vector<std::string>& _fileNames);

protected:
	int			code;
	std::time_t elapsedTime;
	std::string description;

    std::vector<std::string> externalFilenames;
    std::vector<std::pair<std::string, std::string> > filebuffers;
};

#endif // JOBSTATUS_H
