#ifndef DEBUGOUTPUT_H
#define DEBUGOUTPUT_H

#include <string>

namespace DebugOutput
{
    static const int NEVER = 0;
    static const int ON_ANY_ERROR = 1;
    static const int ON_FATAL_ERROR = 2;
    static const int ALWAYS = 3;

    int GetValue(const std::string& stringValue);
}

#endif // DEBUGOUTPUT_H
