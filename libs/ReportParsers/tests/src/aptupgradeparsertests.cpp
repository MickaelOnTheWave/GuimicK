/*
 * GuimmicK
 * Copyright (C) 2024 Guimarães Tecnologia Ltda
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <catch2/catch.hpp>

#include "aptupgradeparsertestfixture.h"

using namespace std;

AptGetUpgradeReport CreateExpectedData()
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

TEST_CASE_METHOD(AptUpgradeParserTestFixture, "Fails gracefully when trying to parse non existing file")
{
   TestParseInexistentFile();
}

TEST_CASE_METHOD(AptUpgradeParserTestFixture, "Parses and retrieves expected data")
{
   const wstring fileToParse = L"apt1.txt";
   const AptGetUpgradeReport expectedData = CreateExpectedData();
   TestParseOk(fileToParse, expectedData);
}
