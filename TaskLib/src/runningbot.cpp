#include "runningbot.h"

#include "filetools.h"
#include "htmlreportcreator.h"
#include "pathtools.h"
#include "stringtools.h"

#include <math.h>

using namespace std;

RunningBot::RunningBot(WorkExecutionManager& _workData)
   : workData(_workData)
{
   validCommands["/help"] = CommandData("Shows this message", std::bind(&RunningBot::ExecuteHelp, this));
   validCommands["/listjobs"] = CommandData("List all jobs of a client", std::bind(&RunningBot::ExecuteListJobs, this));
   validCommands["/listclients"] = CommandData("List all clients handled by me", std::bind(&RunningBot::ExecuteListClients, this));
   validCommands["/whoareyou"] = CommandData("Give you my name", std::bind(&RunningBot::ExecuteWhoAreYou, this));
   validCommands["/whatismyid"] = CommandData("Gives you your Telegram User ID", std::bind(&RunningBot::ExecuteGiveUserId, this));
   validCommands["/info"] = CommandData("Displays debug information", std::bind(&RunningBot::ExecuteShowInfo, this));
   validCommands["/shutdown"] = CommandData("Shuts down agent immediately", std::bind(&RunningBot::ExecuteShutdown, this));
   validCommands["/run"] = CommandData("Execute work list", std::bind(&RunningBot::ExecuteWorkList, this));
   validCommands["/wait"] = CommandData("Makes bot wait for user before starting its work", std::bind(&RunningBot::ExecuteWait, this));
}

RunningBot::~RunningBot()
{
}

void RunningBot::OnStart()
{
   int waitTime = workData.configuration.GetAgent()->GetBotData()->waitTimeBeforeAutorunInS;
   stringstream msg;
   msg << "Hi! I'm starting.";
   if (waitTime < 0)
      msg << " I will wait undefinitely for your commands.";
   else
   {
      msg << " I will wait for " << waitTime << " seconds for your commands.\n";
      msg << "After that, I will automatically run the configured work list and then ";
      msg << "shut down.\n";
      msg << "If you give me any commands, I will execute them but I will keep with my ";
      msg << "original plan to execute the configured work list on configured time and then ";
      msg << "shut down.\n";
      msg << "If you want me to wait, you can send me the /wait command. This will make me ";
      msg << "not execute the work list automatically and wait forever for your commands.";
   }

   SendMessage(msg.str());
}

void RunningBot::OnFinish()
{
   SendMessage("I'm leaving. Bye bye!");
}

void RunningBot::ExecuteHelp()
{
   if (IsUserAuthorized())
   {
      std::string message = "Here are the available commands :\n";
      for (auto command : validCommands)
         message += std::string("\t") + command.first + " :\t" + command.second.description + "\n";

      SendMessage(message);
   }
}

void RunningBot::ExecuteListJobs()
{
   if (IsUserAuthorized())
   {
      std::vector<AbstractJob*> jobs;
      workData.worklist->GetJobList(jobs);

      string message = "Here are all jobs to run for client XXX : \n";
      for (auto job : jobs)
         message += std::string("\t") + StringTools::UnicodeToUtf8(job->GetName()) + "\n";

      SendMessage(message);
   }
}

void RunningBot::ExecuteListClients()
{
   if (IsUserAuthorized())
      SendMessage("Only one client handled in this version");
}

void RunningBot::ExecuteWhoAreYou()
{
   if (IsUserAuthorized())
   {
      const wstring agentName = workData.configuration.GetAgent()->GetName();
      SendMessage(std::string("My name is ") + StringTools::UnicodeToUtf8(agentName));
   }
}

void RunningBot::ExecuteShutdown()
{
   if (IsUserAuthorized())
   {
      if (isRunningWorklist)
         SendMessage("Can't shutdown now : the work is being done. Wait for it to finish.");
      else
      {
         const std::string message = "Ok, I'm shutting down immediatly.\nBye bye!";
         SendMessage(message);
         //ShutdownBot();
      }
   }
}

void RunningBot::ExecuteWorkList()
{
   if (isRunningWorklist)
   {
      SendMessage("Work list currently being processed.\n");
      std::string msg = "Processing job ";
      msg += std::to_string(currentJobIndex) + " of " + std::to_string(jobCount);
      SendMessage(msg);
   }
   else
   {
      delete workThread;
      workThread = new std::thread([this]()
      {
         SendMessage("Starting worklist execution");
         isRunningWorklist = true;

         jobCount = workData.worklist->GetJobCount();
         currentJobIndex = 1;

         AbstractReportCreator* reportCreator = new HtmlReportCreator();
         workData.Run(currentJobIndex, reportCreator);

         SendMessage("Finished worklist execution");

         ReportFileData fileData;
         reportCreator->GetAssociatedFiles(fileData);
         SendExecutionReport(reportCreator->GetReportContent(), fileData);

         /*
         const bool ok = RunLocalShutdown(localShutdown);
         if (ok)
            return (dispatched) ? TM_NO_ERROR : DISPATCH_ERROR;
         else
            return SHUTDOWN_ERROR;*/

         isRunningWorklist = false;
      });
      workThread->detach();
   }
}

void RunningBot::ExecuteWait()
{
   SendMessage("Ok, I won't run on my own. I will wait for your commands.");
   waitForUser = true;
}

void RunningBot::SendExecutionReport(const wstring& reportContent, const ReportFileData& attachments)
{
   const wstring reportFilename = L"guimickReport.html";
   bool ok = FileTools::WriteBufferToFile(reportFilename, reportContent);
   if (ok)
   {
      for (auto bufferAttachment : attachments.fileBuffers)
         ok |= FileTools::WriteBufferToFile(bufferAttachment.first, bufferAttachment.second);
   }

   if (!ok)
      SendMessage("Can't send report : error writing to filesystem.");
   else
   {
      PathTools::GetCurrentFullPath();
      vector<string> filenames;
      filenames.push_back(ToBotPath(reportFilename));
      for (auto attachment : attachments.fileBuffers)
         filenames.push_back(ToBotPath(attachment.first));
      for (auto attachment : attachments.externalFiles)
         filenames.push_back(ToBotPath(attachment));

      SendExecutionReportFiles(filenames);
   }
}

vector<pair<string, string>> RunningBot::ConvertToStr(const vector<pair<wstring, wstring> >& fileBuffers)
{
   vector<pair<string, string>> convertedBuffers;
   for (auto buffer : fileBuffers)
   {
      const string name = StringTools::UnicodeToUtf8(buffer.first);
      const string content = StringTools::UnicodeToUtf8(buffer.second);
      convertedBuffers.push_back(make_pair(name, content));
   }
   return convertedBuffers;
}

string RunningBot::ToBotPath(const wstring& filename)
{
   const std::wstring fullPath = PathTools::GetCurrentFullPath() + L"/" + filename;
   return StringTools::UnicodeToUtf8(fullPath);
}
