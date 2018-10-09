#include "configurationtools.h"

#include <fstream>
#include "configurationobject.h"
#include "jobfactory.h"

using namespace std;

string ConfigurationTools::CreateWarning(const string& message)
{
   return CreateMessage("Warning", message);
}

string ConfigurationTools::CreateError(const string& message)
{
   return CreateMessage("Error", message);
}

void ConfigurationTools::SaveJobListToFile(ofstream& file,
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

template <class T>
void ConfigurationTools::SaveValueToFile(ofstream& file,
                                         const std::string& name,
                                         const T value)
{
   const string tab = "\t";
   file << tab << name << " = \"" << value << "\";" << endl;
}

bool ConfigurationTools::GetBooleanValue(const string &strValue,
                                         vector<string> &errorMessages)
{
   if (strValue == "true")
      return true;
   else if (strValue == "false")
      return false;

   std::string error("Warning : ");
    error += strValue + " is not a valid boolean value. Defaulting to false";
   errorMessages.push_back(error);
   return false;
}

string ConfigurationTools::CreateMessage(const string& tag, const string& message)
{
   if (message != "")
       return tag + " : " + message;
   else
      return string("");
}
/*
template<class T>
void ConfigurationTools::SaveValueToFile(ofstream& file, const string& name, const ConfigurationTools::T value)
{
   const string tab = "\t";
   file << tab << name << " = \"" << value << "\";" << endl;
}*/
