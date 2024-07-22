#include "botfactory.h"

#include "Bot/telegramrunningbot.h"

using namespace std;

RunningBot* BotFactory::Create(WorkExecutionManager& data)
{
   if (data.configuration.GetAgent()->GetBotData()->GetName() == L"TelegramBotData")
   {
      return new TelegramRunningBot(data);
   }
   return NULL;
}
