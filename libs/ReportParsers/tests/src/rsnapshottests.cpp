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
#include "rsnapshotreportparser.h"
#include "tools.h"

using namespace std;

using TestData = vector<pair<wstring, RsnapshotReport>>;

TestData CreateTestData()
{
   TestData data;
   Tools::WStringVec fileListWith5;
   for (int i=0; i<5; ++i)
      fileListWith5.push_back(L"file" + std::to_wstring(i));

   RsnapshotReport expectedReport;
   expectedReport.AddAsAdded(fileListWith5);
   expectedReport.bytesAdded = 123;
   expectedReport.bytesRemoved = 456;
   data.push_back(make_pair(L"rsnapshot5added.log", expectedReport));

   expectedReport.Clear();
   expectedReport.AddAsModified(fileListWith5);
   expectedReport.bytesAdded = 123;
   expectedReport.bytesRemoved = 456;
   data.push_back(make_pair(L"rsnapshot5changed.log", expectedReport));

   expectedReport.Clear();
   expectedReport.AddAsRemoved(fileListWith5);
   expectedReport.bytesAdded = 123;
   expectedReport.bytesRemoved = 456;
   data.push_back(make_pair(L"rsnapshot5removed.log", expectedReport));

   expectedReport.Clear();
   expectedReport.AddAsAdded(Tools::WStringVec{L"new1.pdf",L"new2.pdf",L"new3.pdf"});
   expectedReport.AddAsModified(Tools::WStringVec{L"file2",L"file3",L"file4"});
   expectedReport.AddAsRemoved(Tools::WStringVec{L"file0",L"file1"});
   expectedReport.bytesAdded = 123;
   expectedReport.bytesRemoved = 456;
   data.push_back(make_pair(L"rsnapshotall.log", expectedReport));

   return data;
}

TEST_CASE_METHOD(FileBackupParserTestFixture, "Rsnapshot - Parse OK")
{
   SetParser(new RSnapshotReportParser());
   const wstring dataFolder = L"";

   TestData testData = CreateTestData();
   for (const auto& currentTest : testData)
      TestParseOk(dataFolder + currentTest.first, currentTest.second);
}
