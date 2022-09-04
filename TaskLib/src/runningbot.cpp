#include "runningbot.h"

#include "stringtools.h"

#include <math.h>

RunningBot::RunningBot(Agent* _agent, ClientWorkManager* _worklist)
   : agent(_agent), worklist(_worklist)
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
   SendMessage("Hi! I'm starting...");
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
      worklist->GetJobList(jobs);

      std::string message = "Here are all jobs to run for client XXX : \n";
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
      SendMessage(std::string("My name is ") + StringTools::UnicodeToUtf8(agent->GetName()));
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
         ShutdownBot();
      }
   }
}

void RunningBot::ExecuteWorkList()
{
   if (isRunningWorklist)
   {
      SendMessage("The work is already being done!");
      std::string msg = "tempCounter : ";
      msg += std::to_string(tempCounter);
      SendMessage(msg);
   }
   else
   {
      delete workThread;
      workThread = new std::thread([this]()
      {
         isRunningWorklist = true;
         tempCounter = 0;
         const long maxI = 1000000000;
         for (long i=0; i<maxI; ++i)
         {
            long j = sqrt(i);
            tempCounter = (i * 100) / maxI;
         }
         isRunningWorklist = false;
      });
      workThread->detach();
      SendMessage("TODO : implement execution. For this a FSM is required.");
      /*WorkResultData* workResult = workList->RunWorkList();
      AbstractReportCreator* reportCreator = configuration.GetReportCreator();
      reportCreator->Generate(workResult, configurationErrors, version);
      delete workResult;
      const bool dispatched = DispatchReport(reportCreator, *typedConfiguration, commandLine);
      delete reportCreator;

      const bool ok = RunLocalShutdown(localShutdown);
      if (ok)
         return (dispatched) ? TM_NO_ERROR : DISPATCH_ERROR;
      else
         return SHUTDOWN_ERROR;*/
   }
}

void RunningBot::ExecuteWait()
{
   SendMessage("Ok, I won't run on my own. I will wait for your commands.");
   waitForUser = true;
}
