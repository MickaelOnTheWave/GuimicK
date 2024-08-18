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

#include "filebackupparsertestfixture.h"
#include "tools.h"
#include "tarcommandparser.h"

using namespace std;

using TestData = vector<pair<wstring, FileBackupReport>>;

TestData CreateTarTestData()
{
   TestData data;

   const Tools::WStringVec fullFileList = {
       L"ExtractDataFromGopro.sh",
       L"encomenda cpb.pdf",
       L"synology.pub",
       L"TODOs (2016)"
   };

   FileBackupReport expectedReport;
   expectedReport.AddAsAdded(fullFileList);
   data.push_back(make_pair(L"added4.txt", expectedReport));

   expectedReport.Clear();
   data.push_back(make_pair(L"empty.txt", expectedReport));

   expectedReport.Clear();
   data.push_back(make_pair(L"warningWithNoAdds.txt", expectedReport));

   return data;
}

TEST_CASE_METHOD(FileBackupParserTestFixture, "Tar - Parse OK")
{
   SetParser(new TarCommandParser(L"/bin/tar"));
   const wstring dataFolder = L"TarCommand/";

   TestData testData = CreateTarTestData();
   for (const auto& currentTest : testData)
      TestParseOk(dataFolder + currentTest.first, currentTest.second);
}
