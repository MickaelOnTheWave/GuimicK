#include "botfactory.h"

#include "telegramrunningbot.h"

RunningBot* BotFactory::Create(Agent* agent, ClientWorkManager* workList)
{
   if (agent->GetBotData()->GetName() == L"TelegramBotData")
   {
      return new TelegramRunningBot(agent, workList, agent->GetBotData()->botToken);
   }
   return NULL;
}
