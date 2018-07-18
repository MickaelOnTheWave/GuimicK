#include "debugoutput.h"

namespace DebugOutput
{

int GetValue(const std::string& stringValue)
{
    if (stringValue == "true")
        return ALWAYS;
    else if (stringValue == "false")
        return NEVER;
    else if (stringValue == "always")
        return ALWAYS;
    else if (stringValue == "onanyerror")
        return ON_ANY_ERROR;
    else if (stringValue == "onfatalerror")
        return ON_FATAL_ERROR;
    else if (stringValue == "never")
        return NEVER;
    else
        return NEVER;
}

std::string GetDescription(const int value)
{
   switch (value)
   {
      case UNDEFINED      : return "Undefined";
      case NEVER          : return "Never";
      case ON_ANY_ERROR   : return "On any error";
      case ON_FATAL_ERROR : return "On fatal error";
      case ALWAYS         : return "Always";
      default             : return "Unsupported value";
   }
}

}
