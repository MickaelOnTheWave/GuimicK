#ifndef DISKCOMMANDPARSERTEST_H
#define DISKCOMMANDPARSERTEST_H

#include <string>

#include "abstractdiskcommandparser.h"

struct DiskOutput
{
   std::vector<LogicalDrive> drives;
   std::wstring miniReport;
   std::wstring fullReport;
};

class DiskParserTestFixture
{
public:
   DiskParserTestFixture() = default;
   virtual ~DiskParserTestFixture();

   void TestNoCrashOnInvalidBuffer();

   void TestParseOk(const std::wstring& inputBuffer,
                    const DiskOutput& expectedOutput);

   void TestParseError(const std::wstring& inputBuffer,
                       const DiskOutput& expectedOutput);

protected:
   AbstractDiskCommandParser* parser = nullptr;

private:
   void TestParse(const std::wstring& inputBuffer,
                  const DiskOutput& expectedOutput,
                  const bool expectedResult);

    void CheckDrives(const std::vector<LogicalDrive>& actual,
                     const std::vector<LogicalDrive>& expected);
    void CheckReports(const std::wstring& miniReport,
                      const std::wstring& fullReport);
};

#endif // DISKCOMMANDPARSERTEST_H
