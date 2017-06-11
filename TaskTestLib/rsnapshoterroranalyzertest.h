#ifndef RSNAPSHOTERRORANALYZERTEST_H
#define RSNAPSHOTERRORANALYZERTEST_H

#include "qttestsuite.h"

class RsnapshotErrorAnalyzerTest : public QtTestSuite
{
    Q_OBJECT
public:
     RsnapshotErrorAnalyzerTest(const std::string& dataFolderPrefix);

private Q_SLOTS:
    void init();
    void cleanup();

    void testOutput();
    void testOutput_data();

private:
};

#endif // RSNAPSHOTERRORANALYZERTEST_H
