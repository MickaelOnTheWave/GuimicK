#include "resultcollectionstatus.h"

ResultCollectionStatus::ResultCollectionStatus(WorkResultData* _results)
   : JobStatus(), results(_results)
{
}

ResultCollectionStatus::~ResultCollectionStatus()
{
   delete results;
}

WorkResultData*ResultCollectionStatus::GetResults()
{
   return results;
}
