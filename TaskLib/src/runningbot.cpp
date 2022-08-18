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
}

RunningBot::~RunningBot()
{
}

void RunningBot::ExecuteHelp()
{
   std::string message = "Here are the available commands :\n";
   for (auto command : validCommands)
      message += std::string("\t") + command.first + " :\t" + command.second.description + "\n";

   SendMessage(message);
}

void RunningBot::ExecuteListJobs()
{
   std::vector<AbstractJob*> jobs;
   worklist->GetJobList(jobs);

   std::string message = "Here are all jobs to run for client XXX : \n";
   for (auto job : jobs)
      message += std::string("\t") + StringTools::UnicodeToUtf8(job->GetName()) + "\n";

   SendMessage(message);
}

void RunningBot::ExecuteListClients()
{
   SendMessage("Only one client handled in this version");
}

void RunningBot::ExecuteWhoAreYou()
{
   SendMessage(std::string("My name is ") + StringTools::UnicodeToUtf8(agent->GetName()));
}


