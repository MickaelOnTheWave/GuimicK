#include "debugoutput.h"

namespace DebugOutput
{

int GetValue(const std::wstring& stringValue)
{
    if (stringValue == L"true")
        return ALWAYS;
    else if (stringValue == L"false")
        return NEVER;
    else if (stringValue == L"always")
        return ALWAYS;
    else if (stringValue == L"onanyerror")
        return ON_ANY_ERROR;
    else if (stringValue == L"onfatalerror")
        return ON_FATAL_ERROR;
    else if (stringValue == L"never")
        return NEVER;
    else
        return NEVER;
}

std::wstring GetDescription(const int value)
{
   switch (value)
   {
      case UNDEFINED      : return L"Undefined";
      case NEVER          : return L"Never";
      case ON_ANY_ERROR   : return L"On any error";
      case ON_FATAL_ERROR : return L"On fatal error";
      case ALWAYS         : return L"Always";
      default             : return L"Unsupported value";
   }
}

}
