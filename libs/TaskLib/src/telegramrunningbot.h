#ifndef TELEGRAMRUNNINGBOT_H
#define TELEGRAMRUNNINGBOT_H

#include "runningbot.h"

#include <string>
#include <tgbot/tgbot.h>
#include "TelegramBotData.h"

class TelegramRunningBot : public RunningBot
{
public:
   TelegramRunningBot(WorkExecutionManager& _workManager);

   void LoopRun() override;

protected:
   void SendMessage(const std::string& message) const override;
   void SendExecutionReportFiles(const std::vector<std::string>& filenames) override;

   void ExecuteGiveUserId() override;
   void ExecuteShowInfo() override;
   bool IsUserAuthorized() override;
   void ShutdownBot() override;

private:
   TelegramBotData* GetBotData();
   void SendFile(const std::string& filename, const std::string& mimeType);
   bool IgnoreCommand(const std::string& commandName);

   enum class TimedRunStep
   {
      WaitForTimeout,
      WaitForRunStart,
      WaitForRunFinish
   };

   TimedRunStep currentStep = TimedRunStep::WaitForTimeout;
   bool WaitForSteppedRun(const time_t startTime);

   bool firstMessageReceived = false;
   TgBot::Bot bot;
   TgBot::Message::Ptr currentMessage;
   int64_t chatId;
};

#endif // TELEGRAMRUNNINGBOT_H
