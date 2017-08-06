#include "emaildispatcherfactory.h"

#ifdef USE_CURL_LIB
    #include "curllibreportdispatcher.h"
#else
    #include "curlconsoleemailsender.h"
#endif

EmailReportDispatcher *EmailDispatcherFactory::Create()
{
#ifdef USE_CURL_LIB
    return new CurlLibReportDispatcher();
#else
    return new CurlConsoleReportDispatcher();
#endif
}
