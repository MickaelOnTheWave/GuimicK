#include "botfactory.h"

#include "SelfIdentity.h"
#include "telegramrunningbot.h"
#include "stringtools.h"


RunningBot* BotFactory::Create(const int botMode, const std::wstring& botToken)
{
   if (botMode == Agent::BOTMODE_TELEGRAM)
      return new TelegramRunningBot(StringTools::UnicodeToUtf8(botToken));
   return NULL;
}
