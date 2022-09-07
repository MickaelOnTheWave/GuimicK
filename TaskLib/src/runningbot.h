#ifndef RUNNINGBOT_H
#define RUNNINGBOT_H

#include <functional>
#include <map>
#include <string>
#include <thread>

#include "WorkExecutionManager.h"

class CommandData
{
public:
   CommandData() : description("") {}
   CommandData(const std::string& _description, std::function<void()> _functor)
      : description(_description), functor(_functor) {}

   std::string description;
   std::function<void()> functor;
};

class RunningBot
{
public:
   RunningBot(WorkExecutionManager& _workData);
   virtual ~RunningBot();

   virtual void LoopRun() = 0;

protected:
   void OnStart();
   void OnFinish();

   void ExecuteHelp();
   void ExecuteListJobs();
   void ExecuteListClients();
   void ExecuteWhoAreYou();
   void ExecuteShutdown();
   void ExecuteWorkList();
   void ExecuteWait();
   virtual void ExecuteGiveUserId() = 0;
   virtual void ExecuteShowInfo() = 0;
   virtual bool IsUserAuthorized() = 0;
   virtual void ShutdownBot() = 0;

   virtual void SendMessage(const std::string& message) const = 0;
   virtual void SendExecutionReportFiles(const std::vector<std::string>& filenames) = 0;

   WorkExecutionManager& workData;

   std::map<std::string, CommandData> validCommands;
   bool waitForUser = false;

   bool isRunningWorklist = false;
   std::thread *workThread = nullptr;
   int jobCount = 0;
   int currentJobIndex = 0;

private :
   void SendExecutionReport(const std::wstring& reportContent, const ReportFileData& attachments);
   static std::vector<std::pair<std::string,std::string>> ConvertToStr(const std::vector<std::pair<std::wstring,std::wstring>>& fileBuffers);
   static std::string ToBotPath(const std::wstring& filename);
};

#endif // RUNNINGBOT_H
