#include "emaildispatcherfactory.h"

//#include "curllibemailsender.h"
#include "curlconsoleemailsender.h"

EmailReportDispatcher *EmailDispatcherFactory::Create()
{
    return new CurlConsoleEmailSender();
}
