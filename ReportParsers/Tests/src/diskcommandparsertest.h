#ifndef DISKCOMMANDPARSERTEST_H
#define DISKCOMMANDPARSERTEST_H

#include <gtest/gtest.h>
#include <string>

#include "abstractdiskcommandparser.h"

class DiskCommandParserTest : public ::testing::Test
{
public:
   DiskCommandParserTest() = default;

   void TearDown();

   void TestNoCrashOnInvalidBuffer();
   void TestParseOk(const std::wstring& inputBuffer,
                    const std::vector<LogicalDrive>& expectedDrives,
                    const std::wstring& miniReport,
                    const std::wstring& fullReport);

protected:
   AbstractDiskCommandParser* parser = nullptr;

private:
    void CheckDrives(const std::vector<LogicalDrive>& actual,
                     const std::vector<LogicalDrive>& expected);
    void CheckReports(const std::wstring& miniReport,
                      const std::wstring& fullReport);
};

#endif // DISKCOMMANDPARSERTEST_H
