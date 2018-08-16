#ifndef DUMMYJOB_H
#define DUMMYJOB_H

#include <string>
#include "abstractjob.h"

class DummyJob : public AbstractJob
{
public:
    DummyJob(const std::string& _name = "DummyJob",
             const int expectedCode = JobStatus::OK,
             const std::string& _expectedDescription = "");
    DummyJob(const DummyJob& other);

    virtual AbstractJob* Clone();

    virtual bool InitializeFromClient(Client* client);

    virtual bool IsInitialized(void);

	virtual JobStatus* Run();

    void SetStatusDescription(const std::string& value);

    void SetInitialization(const bool value);

protected:
    int statusCode;
    std::string statusDescription;
    bool initializationValue;
};

#endif // DUMMYJOB_H
