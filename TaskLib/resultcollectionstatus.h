#ifndef RESULTCOLLECTIONSTATUS_H
#define RESULTCOLLECTIONSTATUS_H

#include "jobstatus.h"
#include "workresultdata.h"

class ResultCollectionStatus : public JobStatus
{
public:
   ResultCollectionStatus(WorkResultData* _results);
   virtual ~ResultCollectionStatus();

   WorkResultData* GetResults();

private:
   WorkResultData* results;
};

#endif // RESULTCOLLECTIONSTATUS_H
