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
#include "gitplumbingreportparser.h"
#include "gitporcelainreportparser.h"
#include "tools.h"

using namespace std;

using TestData = vector<pair<wstring, FileBackupReport>>;

TestData CreatePlumbingTestData()
{
   TestData data;
   const Tools::WStringVec fullFileList = {
       L"6232100495809.sav",
       L"GamerProfile.xml",
       L"TODOs",
       L"TODOs (2016)",
       L"ex1_description.txt"
   };

   FileBackupReport expectedReport;
   expectedReport.AddAsModified(fullFileList);
   data.push_back(make_pair(L"modified5.txt", expectedReport));

   expectedReport.Clear();
   expectedReport.AddAsRemoved(fullFileList);
   data.push_back(make_pair(L"removed5.txt", expectedReport));

   expectedReport.Clear();
   expectedReport.AddAsAdded(Tools::WStringVec{L"HomepageEsperancajpg"});
   expectedReport.AddAsModified(Tools::WStringVec{L"GamerProfile.xml", L"ex1_description.txt"});
   expectedReport.AddAsRemoved(Tools::WStringVec{L"TODOs", L"TODOs (2016)"});
   data.push_back(make_pair(L"allchanges.txt", expectedReport));

   return data;
}

TestData CreatePorcelainTestData()
{
   TestData data;
   Tools::WStringVec fileListWith5;
   for (int i=0; i<5; ++i)
      fileListWith5.push_back(L"file" + std::to_wstring(i));


   FileBackupReport expectedReport;
   expectedReport.AddAsAdded(fileListWith5);
   data.push_back(make_pair(L"gitadd5.log", expectedReport));

   expectedReport.Clear();
   expectedReport.AddAsModified(fileListWith5);
   data.push_back(make_pair(L"gitmod5.log", expectedReport));

   expectedReport.Clear();
   expectedReport.AddAsRemoved(fileListWith5);
   data.push_back(make_pair(L"gitrm5.log", expectedReport));

   expectedReport.Clear();
   expectedReport.AddAsAdded(Tools::WStringVec{L"alternate0",L"alternate1",L"alternate2"});
   expectedReport.AddAsModified(Tools::WStringVec{L"file0",L"file1",L"file2"});
   expectedReport.AddAsRemoved(Tools::WStringVec{L"file3",L"file4"});
   data.push_back(make_pair(L"gitallchanges.log", expectedReport));

   return data;
}

TEST_CASE_METHOD(FileBackupParserTestFixture, "Git Plumbing - Parse OK")
{
   SetParser(new GitPlumbingReportParser());
   const wstring dataFolder = L"GitPlumbing/";

   TestData testData = CreatePlumbingTestData();
   for (const auto currentTest : testData)
      TestParseOk(dataFolder + currentTest.first, currentTest.second);
}

TEST_CASE_METHOD(FileBackupParserTestFixture, "Git Porcelain - Parse OK")
{
   SetParser(new GitPorcelainReportParser());
   const wstring dataFolder = L"GitPull/";

   TestData testData = CreatePorcelainTestData();
   for (const auto currentTest : testData)
      TestParseOk(dataFolder + currentTest.first, currentTest.second);
}
