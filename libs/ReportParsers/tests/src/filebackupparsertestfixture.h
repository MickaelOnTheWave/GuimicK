#ifndef FILEBACKUPPARSERTESTFIXTURE_H
#define FILEBACKUPPARSERTESTFIXTURE_H

#include "abstractoutputparsertestfixture.h"

#include <string>
#include "filebackupreport.h"

class FileBackupParserTestFixture : public AbstractOutputParserTestFixture
{
public:
    FileBackupParserTestFixture(const std::wstring& dataPrefix);
    void TestParseOk(const std::wstring &file, const FileBackupReport &expectedData);
};

#endif // FILEBACKUPPARSERTESTFIXTURE_H
