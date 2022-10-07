#ifndef BOTDATA_H
#define BOTDATA_H

#include <string>

struct BotData
{
   virtual std::wstring GetName() const
   {
      return L"BotData";
   }

   int waitTimeBeforeAutorunInS;
};

#endif // BOTDATA_H
