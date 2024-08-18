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
#include "rsynccommandparser.h"

using namespace std;

using TestData = vector<pair<wstring, FileBackupReport>>;

TestData CreateRsyncTestData()
{
   TestData data;

   FileBackupReport expectedReport;
   expectedReport.AddAsAdded({L"wooo", L"yaahaaaa", L"yiiiii", L"yoohoo"});
   data.push_back(make_pair(L"add4", expectedReport));

   expectedReport.Clear();
   expectedReport.AddAsAdded(L"onemorefilenow");
   data.push_back(make_pair(L"add1", expectedReport));

   expectedReport.Clear();
   expectedReport.AddAsAdded({L"New Folder/", L"New Folder/always_coke.jpg",
                              L"New Folder/elections1.gif", L"New Folder/joie_bureau.gif",
                              L"New Folder/manque_morceau.gif"});
   data.push_back(make_pair(L"addfolder", expectedReport));

   expectedReport.Clear();
   expectedReport.AddAsModified({L"onemorefilenow", L"wooo", L"yaahaaaa",
                                 L"yiiiii", L"yoohoo"});
   data.push_back(make_pair(L"change5", expectedReport));

   expectedReport.Clear();
   expectedReport.AddAsRemoved({L"yiiiii", L"yaahaaaa", L"wooo", L"onemorefilenow"});
   data.push_back(make_pair(L"remove4", expectedReport));

   expectedReport.Clear();
   expectedReport.AddAsAdded(Tools::WStringVec{L"On ne change pas", L"empty folder/"});
   expectedReport.AddAsModified(L"onemorefilenow");
   expectedReport.AddAsRemoved(Tools::WStringVec{L"file0",L"file1"});
   data.push_back(make_pair(L"mixed", expectedReport));

   return data;
}

TEST_CASE_METHOD(FileBackupParserTestFixture, "Rsync - Parse OK")
{
   SetParser(new RsyncCommandParser());
   const wstring dataFolder = L"RsyncCommand/";

   TestData testData = CreateRsyncTestData ();
   for (const auto& currentTest : testData)
      TestParseOk(dataFolder + currentTest.first, currentTest.second);
}
