#ifndef JOBSTATUS_H
#define JOBSTATUS_H

#include <ctime>
#include <map>
#include <string>
#include <vector>

class JobStatus
{
public:
   static const int NotExecuted;
   static const int Ok;
   static const int OkWithWarnings;
   static const int Error;

   JobStatus();
   JobStatus(int _code, const std::wstring& _description = L"");
   JobStatus(const JobStatus& other);
   virtual ~JobStatus();

   void Reset();

   int GetCode() const;
   void SetCode(int _code);

   static std::wstring GetCodeDescription(int _code);
   static int GetCodeFromDescription(const std::wstring& _description);
   std::wstring GetCodeDescription() const;

   std::time_t GetDuration() const;
   void SetDuration(std::time_t duration);

   virtual std::wstring GetDescription() const;
   void SetDescription(const std::wstring& _description);

    void AddExternalFile(const std::wstring& filename);
    void AddFileBuffer(const std::wstring& filename, const std::wstring& filecontents);
    void AddExternalFilesFromStatus(JobStatus* other);
    void AddFileBuffersFromStatus(JobStatus* other);
    void AddAllFilesFromStatus(JobStatus* other);
    void ClearAllFiles();

    bool IsWorseThan(int otherCode) const;

    /**
            A status is considered fatal if no processing can be done based
            on its execution (Error or NotExecuted for example).
      */
    bool IsFatal() const;
    bool IsOk() const;

    std::wstring ToString() const;


    bool HasExternalFiles();

    typedef std::vector<std::pair<std::wstring,std::wstring> > FileBufferList;
    void GetExternalFilenames(std::vector<std::wstring>& _fileNames);
    void GetFileBuffers(FileBufferList& _filebuffers);

protected:
    int code;
    std::time_t elapsedTime;
    std::wstring description;

   std::vector<std::wstring> externalFilenames;
   std::vector<std::pair<std::wstring, std::wstring> > filebuffers;

private:
   static void PopulateCodeMap();
   static std::map<int, std::wstring> codeMap;
};

#endif // JOBSTATUS_H
