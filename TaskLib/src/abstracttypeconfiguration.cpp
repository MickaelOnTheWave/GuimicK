#include "abstracttypeconfiguration.h"

using namespace std;

AbstractTypeConfiguration::AbstractTypeConfiguration() : hasFatalError(false)
{
}

AbstractTypeConfiguration::~AbstractTypeConfiguration()
{
}

bool AbstractTypeConfiguration::Load(ConfigurationParser& parser,
                                     vector<string>& errorMessages)
{
   FillRootObjects(parser.objectList, errorMessages);
   FillGlobalProperties(parser.anonymousObject, errorMessages);
   return IsConfigurationConsistent(errorMessages);
}
