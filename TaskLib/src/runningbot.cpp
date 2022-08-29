#include "runningbot.h"

#include "stringtools.h"

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
}

RunningBot::~RunningBot()
{
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
      const std::string message = "Ok, I'm shutting down immediatly.\nBye bye!";
      SendMessage(message);
      ShutdownBot();
   }
}


