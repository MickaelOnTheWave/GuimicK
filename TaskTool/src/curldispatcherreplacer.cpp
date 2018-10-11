#include "curldispatcherreplacer.h"

#include "emaildispatcherfactory.h"

AbstractReportDispatcher *CurlDispatcherReplacer::Run(AbstractReportDispatcher *input,
                                                      const StandaloneConfiguration &configuration)
{
    // For linking reasons, email dispatcher creation is being done in tool, not in
    // tasklib. Here is the workaround to get the final dispatcher : replace the dummy
    // one returned from configuration to the real one linked in tool.

    if (input->GetName() == "Dummy Email")
    {
        delete input;
        AbstractReportDispatcher* trueDispatcher = EmailDispatcherFactory::Create();
        trueDispatcher->Initialize(&configuration);
        return trueDispatcher;
    }
    else
        return input;
}
