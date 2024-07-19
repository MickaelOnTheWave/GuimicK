#include "configurationtools.h"

#include "configurationobject.h"
#include "jobfactory.h"

using namespace std;

wstring ConfigurationTools::CreateWarning(const wstring& message)
{
   return CreateMessage(L"Warning", message);
}

wstring ConfigurationTools::CreateError(const wstring& message)
{
   return CreateMessage(L"Error", message);
}

wstring ConfigurationTools::CreateUnhandledProperty(const wstring& property)
{
   const wstring warningMessage = L"Warning : unhandled property ";
   return warningMessage + property;
}

void ConfigurationTools::SaveJobListToFile(wofstream& file,
                                           const list<AbstractJob*>& jobList)
{
   JobFactory jobFactory;
   file << "JobList" << endl;
   file << "{" << endl;

   list<AbstractJob*>::const_iterator it = jobList.begin();
   for (; it != jobList.end(); ++it)
   {
      ConfigurationObject* confObject = jobFactory.CreateConfigurationObject(*it);
      if (confObject)
      {
         file << confObject->CreateConfigurationString(1) << endl;
         delete confObject;
      }
   }

   file << "}" << endl;
}

bool ConfigurationTools::GetBooleanValue(const wstring &strValue,
                                         vector<wstring> &errorMessages)
{
   if (strValue == L"true")
      return true;
   else if (strValue == L"false")
      return false;

   std::wstring error(L"Warning : ");
    error += strValue + L" is not a valid boolean value. Defaulting to false";
   errorMessages.push_back(error);
   return false;
}

wstring ConfigurationTools::CreateMessage(const wstring& tag, const wstring& message)
{
   if (message != L"")
       return tag + L" : " + message;
   else
      return L"";
}

