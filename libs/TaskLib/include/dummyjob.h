#ifndef DUMMYJOB_H
#define DUMMYJOB_H

#include <string>
#include "abstractjob.h"

class DummyJob : public AbstractJob
{
public:
    DummyJob(const std::wstring& _name = L"DummyJob",
             const int expectedCode = JobStatus::Ok,
             const std::wstring& _expectedDescription = L"");
    DummyJob(const DummyJob& other);

    virtual AbstractJob* Clone();

    virtual bool InitializeFromClient(Client* client);

    virtual bool IsInitialized(void);

	virtual JobStatus* Run();

    void SetStatusDescription(const std::wstring& value);

    void SetInitialization(const bool value);

protected:
    int statusCode;
    std::wstring statusDescription;
    bool initializationValue;
};

#endif // DUMMYJOB_H
