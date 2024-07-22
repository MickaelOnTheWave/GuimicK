#ifndef TELEGRAMBOTDATA_H
#define TELEGRAMBOTDATA_H

#include "BotData.h"

struct TelegramBotData : public BotData
{
   std::wstring GetName() const override
   {
      return L"TelegramBotData";
   }

   std::string botToken;
   std::string authorizedUserToken;
   std::string authorizedUserChatId;
};

#endif // TELEGRAMBOTDATA_H
