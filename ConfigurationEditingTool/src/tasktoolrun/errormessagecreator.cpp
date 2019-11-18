#include "errormessagecreator.h"

ErrorMessageCreator::ErrorMessageCreator()
{

}

QString ErrorMessageCreator::CreateMessage(
      const TaskToolConfigurationData& configurationData,
      const TaskToolRunData& runData)
{
   return (runData.isOk) ? QString::fromStdWString(runData.output)
                         : CreateExecutionErrorMessage(configurationData, runData);
}

QString ErrorMessageCreator::CreateExecutionErrorMessage(
      const TaskToolConfigurationData& configurationData,
      const TaskToolRunData& runData)
{
   if (runData.returnCode == 1)
       return CreateConfigurationFileErrorMessage(configurationData);
   else
       return CreateGenericExecutionErrorMessage(configurationData, runData);
}

QString ErrorMessageCreator::CreateConfigurationFileErrorMessage(
      const TaskToolConfigurationData& configurationData) const
{
    QString errorMessage = "Error running Task tool command : ";
    errorMessage += "Configuration file not found.\n";
    errorMessage += "\tConfiguration file : " + configurationData.configurationFile + "\n";
    errorMessage += "\tRun Path : " + configurationData.runPath + "\n";
    errorMessage += "\tExecutable : " + configurationData.executable + "\n";
    return errorMessage;
}

QString ErrorMessageCreator::CreateGenericExecutionErrorMessage(
      const TaskToolConfigurationData& configurationData,
      const TaskToolRunData& runData) const
{
    QString errorMessage = "Error running Task tool command.\n";
    errorMessage += "\tError code : " + QString::number(runData.returnCode) + "\n";
    errorMessage += "\tOutput : " + QString::fromStdWString(runData.output) + "\n";
    errorMessage += "\tRun Path : " + configurationData.runPath + "\n";
    errorMessage += "\tExecutable : " + configurationData.executable + "\n";
    errorMessage += "\tConfiguration file : " + configurationData.configurationFile + "\n";
    return errorMessage;
}
