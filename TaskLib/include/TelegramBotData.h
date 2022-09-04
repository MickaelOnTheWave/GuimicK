#ifndef TELEGRAMBOTDATA_H
#define TELEGRAMBOTDATA_H

#include <string>

class TelegramBotData
{
public :
   virtual std::wstring GetName() const
   {
      return L"TelegramBotData";
   }

   std::string botToken;
   std::string authorizedUserToken;
   std::string authorizedUserChatId;
};

#endif // TELEGRAMBOTDATA_H
