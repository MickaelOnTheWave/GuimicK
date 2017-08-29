#include "debugoutput.h"

int DebugOutput::GetValue(const std::string& stringValue)
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
