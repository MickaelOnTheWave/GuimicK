#ifndef LIBWAKEJOB_H
#define LIBWAKEJOB_H

#include "abstractwakejob.h"

class LibWakeJob : public AbstractWakeJob
{
public:
   LibWakeJob();
   LibWakeJob(const LibWakeJob& other);
   virtual ~LibWakeJob();

   virtual AbstractJob* Clone();

private:
    virtual JobStatus* SetupWaker();
    virtual JobStatus* RunWaker();
};

#endif // LIBWAKEJOB_H
