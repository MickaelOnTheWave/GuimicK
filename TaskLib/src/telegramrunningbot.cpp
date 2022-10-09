#include "telegramrunningbot.h"

#include <tgbot/net/CurlHttpClient.h>
#include <tgbot/net/HttpReqArg.h>
#include <tgbot/net/Url.h>

TelegramRunningBot::TelegramRunningBot(WorkExecutionManager& _workManager)
   : RunningBot(_workManager),
     bot(GetBotData()->botToken)
{
   chatId = std::stoll(GetBotData()->authorizedUserChatId);

   bot.getEvents().onCommand("", [this](TgBot::Message::Ptr message) {
   });
   bot.getEvents().onAnyMessage([this](TgBot::Message::Ptr message)
   {
      auto itCommand = validCommands.find(message->text);
      if (itCommand != validCommands.end())
      {
         if (!IgnoreCommand(itCommand->first))
         {
            currentMessage = message;
            itCommand->second.functor();
         }
      }
      else
         bot.getApi().sendMessage(message->chat->id, "Unknown command : " + message->text);
   });
}

void TelegramRunningBot::LoopRun()
{
   try
   {
      OnStart();

      TgBot::TgLongPoll longPoll(bot);

      time_t startTime = time(NULL);
      while (!forceBotShutdown && (waitForUser || WaitForSteppedRun(startTime)))
      {
         longPoll.start();
      }

      OnFinish();
   } catch (TgBot::TgException& e) {
       printf("error: %s\n", e.what());
   }   
}

void TelegramRunningBot::SendMessage(const std::string& message) const
{
   bot.getApi().sendMessage(chatId, message);
}

void TelegramRunningBot::SendExecutionReportFiles(const std::vector<std::string>& filenames)
{
   if (!filenames.empty())
   {
      auto it = filenames.begin();
      SendFile(*it, "text/html");
      ++it;

      for (; it != filenames.end(); ++it)
      {
         SendFile(*it, "text/plain");
      }
   }
}

bool TelegramRunningBot::IsUserAuthorized()
{
   const bool authorized = (std::to_string(currentMessage->from->id) == GetBotData()->authorizedUserToken);
   if (!authorized)
      bot.getApi().sendMessage(currentMessage->chat->id, "I don't know you. Who are you?");
   return authorized;
}

void TelegramRunningBot::ShutdownBot()
{
   forceBotShutdown = true;
}

TelegramBotData* TelegramRunningBot::GetBotData()
{
   return workData.configuration.GetAgent()->GetBotData();
}

void TelegramRunningBot::SendFile(const std::string& filename, const std::string& mimeType)
{
   TgBot::InputFile::Ptr file = TgBot::InputFile::fromFile(filename, mimeType);
   bot.getApi().sendDocument(chatId, file);
}

bool TelegramRunningBot::IgnoreCommand(const std::string& commandName)
{
   // Workaround for bug where shutdown command is not "cleared" from the list
   // of pending messages after being executed.
   // This causes all subsequent runs to immediately shutdown.
   if (!firstMessageReceived)
   {
      firstMessageReceived = true;
      if (commandName == "/shutdown")
         return true;
   }
   return false;
}

bool TelegramRunningBot::WaitForSteppedRun(const time_t startTime)
{
   if (currentStep == TimedRunStep::WaitForTimeout)
   {
      time_t elapsedTime = time(NULL) - startTime;
      if (elapsedTime >= GetBotData()->waitTimeBeforeAutorunInS)
      {
         ExecuteWorkList();
         currentStep = TimedRunStep::WaitForRunStart;
      }
      return true;
   }
   return false;
}

void TelegramRunningBot::ExecuteGiveUserId()
{
   std::string message = std::string("Hey ") + currentMessage->from->firstName + ", your id is ";
   message += std::to_string(currentMessage->from->id);
   bot.getApi().sendMessage(currentMessage->chat->id, message);
}

void TelegramRunningBot::ExecuteShowInfo()
{
   std::string message = std::string("Here is some internal data about myself :\n");
   message += std::string("\tTelegram Data about myself :\n");
   message += std::string("\t\tUsername :") + bot.getApi().getMe()->username + "\n";
   message += std::string("\t\tFirst name :") + bot.getApi().getMe()->firstName + "\n";
   message += std::string("\t\tLast name :") + bot.getApi().getMe()->lastName + "\n";
   message += std::string("\t\tid :") + std::to_string(bot.getApi().getMe()->id) + "\n";
   message += std::string("\t\tLanguage code :") + bot.getApi().getMe()->languageCode + "\n";
   message += std::string("\t\tAm I a bot :") + std::to_string(bot.getApi().getMe()->isBot) + "(You had a doubt? :-) )\n";
   message += std::string("\tTelegram Data about *YOU* :\n");
   message += std::string("\t\tUsername :") + currentMessage->from->username + "\n";
   message += std::string("\t\tFirst name :") + currentMessage->from->firstName + "\n";
   message += std::string("\t\tLast name :") + currentMessage->from->lastName + "\n";
   message += std::string("\t\tid :") + std::to_string(currentMessage->from->id) + "\n";
   message += std::string("\t\tAre you a bot? :") + std::to_string(currentMessage->from->isBot) + "\n";
   message += std::string("\tTelegram Data about us :\n");
   message += std::string("\t\tUsername :") + currentMessage->chat->username + "\n";
   message += std::string("\t\tFirst name :") + currentMessage->chat->firstName + "\n";
   message += std::string("\t\tLast name :") + currentMessage->chat->lastName + "\n";
   message += std::string("\t\tid :") + std::to_string(currentMessage->chat->id) + "\n";

   bot.getApi().sendMessage(currentMessage->chat->id, message);
}
