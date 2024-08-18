#include "aptupgradeparsertest.h"

#include <catch2/catch.hpp>
#include "aptgetupgradeparser.h"

using namespace std;

AptUpgradeParserTest::AptUpgradeParserTest(const wstring &dataPrefix)
  : AbstractOutputParserTest(dataPrefix)
{
   parserToTest = new AptGetUpgradeParser();
}

void AptUpgradeParserTest::testParse()
{
   const AptGetUpgradeReport expectedData = CreateExpectedData();

   const bool returnValue = parserToTest->ParseFile(dataFolder + L"apt1.txt");
   REQUIRE(returnValue == true);

   auto typedParser = dynamic_cast<AptGetUpgradeParser*>(parserToTest);
   REQUIRE(typedParser != nullptr);

   AptGetUpgradeReport returnedData;
   typedParser->GetReport(returnedData);
   REQUIRE(returnedData == expectedData);
}

AptGetUpgradeReport AptUpgradeParserTest::CreateExpectedData() const
{
   AptGetUpgradeReport data;
   data.obsoletePackages = {L"libcuda1-331",L"nvidia-331-uvm",L"nvidia-340-uvm",
                            L"nvidia-opencl-icd-331"};
   data.keptPackages =     {L"icedtea-7-jre-jamvm", L"linux-generic", L"linux-headers-generic",
                            L"linux-image-generic", L"linux-signed-generic",
                            L"linux-signed-image-generic", L"openjdk-7-jre",
                            L"openjdk-7-jre-headless"};
   data.upgradedPackages = {L"apt", L"apt-transport-https", L"apt-utils", L"curl",
                            L"firefox", L"firefox-locale-en", L"firefox-locale-fr",
                            L"firefox-locale-pt", L"libapt-inst1.5", L"libapt-pkg4.12",
                            L"libcurl3", L"libcurl3-gnutls", L"libcurl4-openssl-dev",
                            L"libmysqlclient18", L"libmysqlclient18:i386",
                            L"mysql-client-core-5.5", L"mysql-common", L"mysql-server-core-5.5"};
   data.installedPackages = {L"apt", L"apt-transport-https", L"apt-utils", L"curl",
                             L"firefox", L"firefox-locale-en", L"firefox-locale-fr",
                             L"firefox-locale-pt", L"libapt-inst1.5", L"libapt-pkg4.12",
                             L"libcurl3", L"libcurl3-gnutls", L"libcurl4-openssl-dev",
                             L"libmysqlclient18", L"libmysqlclient18:i386",
                             L"mysql-client-core-5.5", L"mysql-common", L"mysql-server-core-5.5"};
   data.removedPackages = {};
   data.updateFileSize = L"52,1 MB";
   return data;
}
