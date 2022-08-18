#include "runningbot.h"

RunningBot::RunningBot(const std::string& _botToken)
   : botToken(_botToken)
{
   validCommands["/listjobs"] = CommandData("Here are the jobs scheduled to run");
   validCommands["/listclients"] = CommandData("Here are the clients handled by me");
   validCommands["/whoareyou"] = CommandData("My name is");
   validCommands["/dontshutdown"] = CommandData("Ok, I will keep running when I finish my tasks");
}

RunningBot::~RunningBot()
{
}
