#include <gtest/gtest.h>

#include "dfparserfixture.h"

using namespace std;

namespace
{
   LogicalDrive BuildDrive(const std::wstring& _name,
                           const std::wstring& _totalSpace,
                           const std::wstring& _usedSpace,
                           const std::wstring& _ratio)
   {
      LogicalDrive drive;
      drive.name = _name;
      drive.totalSpace = _totalSpace;
      drive.usedSpace = _usedSpace;
      drive.ratio = _ratio;
      return drive;
   }
}

TEST_F(DfParserFixture, doesnt_crash_when_parsing_invalid_buffer)
{
   TestNoCrashOnInvalidBuffer();
}

TEST_F(DfParserFixture, creates_one_drive_report)
{
   const wstring buffer = L"Sist.fichs      Tama  Ocup Livre Uso% Montado em \n"
                          L"/dev/sda2       230G  168G   50G  78% /";

   LogicalDrive expectedDrive = BuildDrive(L"/dev/sda2", L"230 Gb", L"168 Gb", L"78%");
   const wstring expectedMiniReport = L"50 Gb available (78% used)";
   const wstring expectedFullReport = L"";
   const DiskOutput expectedOutput = {{expectedDrive}, expectedMiniReport, expectedFullReport};

   TestParseOk(buffer, expectedOutput);
}

TEST_F(DfParserFixture, creates_multiple_drive_report)
{
   const wstring buffer = L"Sist.fichs      Tama  Ocup Livre Uso% Montado em\n"
                          L"udev             12G  4.0K   12G   1% /dev\n"
                          L"tmpfs           2.4G  1.6M  2.4G   1% /run\n"
                          L"/dev/sda2       230G  168G   50G  78% /\n"
                          L"none            4.0K     0  4.0K   0% /sys/fs/cgroup\n"
                          L" none            5.0M     0  5.0M   0% /run/lock\n"
                          L"none             12G   76K   12G   1% /run/shm\n"
                          L"none            100M   20K  100M   1% /run/user\n"
                          L"/dev/sda1        37M  6.3M   31M  18% /boot/efi\n"
                          L"/dev/sdb1       334G  268G   67G  81% /mnt/datadrive\n"
                          L"/dev/sdc2       962G   72M  913G   1% /mnt/linuxdata";

   const vector<LogicalDrive> expectedDrives = {
      BuildDrive(L"/dev/sda2", L"230 Gb", L"168 Gb", L"78%"),
      BuildDrive(L"/dev/sda1", L"37 Mb", L"6.3 Mb", L"18%"),
      BuildDrive(L"/dev/sdb1", L"334 Gb", L"268 Gb", L"81%"),
      BuildDrive(L"/dev/sdc2", L"962 Gb", L"72 Mb", L"1%")
   };

   const wstring expectedMiniReport = L"4 drives checked, see report";
   const wstring expectedFullReport = L"Drive List\n"
                                      L"\t/dev/sda2 : 230 Gb total, 50 Gb available (78% used)\n"
                                      L"\t/dev/sda1 : 37 Mb total, 31 Mb available (18% used)\n"
                                      L"\t/dev/sdb1 : 334 Gb total, 67 Gb available (81% used)\n"
                                      L"\t/dev/sdc2 : 962 Gb total, 913 Gb available (1% used)\n\n";

   const DiskOutput expectedOutput = {expectedDrives, expectedMiniReport, expectedFullReport};

   TestParseOk(buffer, expectedOutput);
}

TEST_F(DfParserFixture, reports_one_drive_not_found_error)
{
   const wstring buffer = L"Filesystem                Size      Used Available Use% Mounted on\n"
                          L"df: /dev/sda2: can't find mount point";

   vector<LogicalDrive> expectedDrives = {};
   const wstring expectedMiniReport = L"Drive /dev/sda2 not found";
   const wstring expectedFullReport = L"";
   const DiskOutput expectedOutput = {expectedDrives, expectedMiniReport, expectedFullReport};

   TestParseOk(buffer, expectedOutput);
}

