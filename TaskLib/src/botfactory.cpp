#include "botfactory.h"

#include "SelfIdentity.h"
#include "telegramrunningbot.h"


RunningBot* BotFactory::Create(const int botMode)
{
   if (botMode == Agent::BOTMODE_TELEGRAM)
      return new TelegramRunningBot();
   return NULL;
}
