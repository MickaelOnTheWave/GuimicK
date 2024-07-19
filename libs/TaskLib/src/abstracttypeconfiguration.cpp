#include "abstracttypeconfiguration.h"

using namespace std;

AbstractTypeConfiguration::AbstractTypeConfiguration() : hasFatalError(false)
{
}

AbstractTypeConfiguration::AbstractTypeConfiguration(const AbstractTypeConfiguration& other)
   : hasFatalError(other.hasFatalError)
{
}

AbstractTypeConfiguration::~AbstractTypeConfiguration()
{
}

bool AbstractTypeConfiguration::Load(ConfigurationParser& parser,
                                     vector<wstring>& errorMessages)
{
   FillRootObjects(parser.objectList, errorMessages);
   FillGlobalProperties(parser.anonymousObject, errorMessages);
   return IsConfigurationConsistent(errorMessages);
}
