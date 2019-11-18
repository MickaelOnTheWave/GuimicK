#ifndef ERRORMESSAGECREATOR_H
#define ERRORMESSAGECREATOR_H

#include <QString>

#include "tasktooldata.h"

class ErrorMessageCreator
{
public:
   ErrorMessageCreator();

   QString CreateMessage(
       const TaskToolConfigurationData& configurationData,
       const TaskToolRunData& runData
   );

private:
   QString CreateExecutionErrorMessage(
         const TaskToolConfigurationData& configurationData,
         const TaskToolRunData& runData
     );

   QString CreateConfigurationFileErrorMessage(
         const TaskToolConfigurationData& configurationData
     ) const;

   QString CreateGenericExecutionErrorMessage(
         const TaskToolConfigurationData& configurationData,
         const TaskToolRunData& runData
     ) const;
};

#endif // ERRORMESSAGECREATOR_H
