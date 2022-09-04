#ifndef RUNNINGBOT_H
#define RUNNINGBOT_H

#include <functional>
#include <map>
#include <string>
#include <thread>

#include "SelfIdentity.h"
#include "clientworkmanager.h"

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
   RunningBot(Agent* _agent, ClientWorkManager* _worklist);
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

   Agent* agent;
   ClientWorkManager* worklist;
   std::map<std::string, CommandData> validCommands;
   bool waitForUser = false;
   bool isRunningWorklist = false;
   std::thread *workThread = nullptr;
   int tempCounter = 0;
};

#endif // RUNNINGBOT_H
