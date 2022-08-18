#include "botfactory.h"

#include "telegramrunningbot.h"

RunningBot* BotFactory::Create(Agent* agent, ClientWorkManager* workList, const std::string& botToken)
{
   if (agent->GetBotMode() == Agent::BOTMODE_TELEGRAM)
      return new TelegramRunningBot(agent, workList, botToken);
   return NULL;
}
