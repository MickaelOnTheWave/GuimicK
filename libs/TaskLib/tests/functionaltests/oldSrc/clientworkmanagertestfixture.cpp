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

#include "clientworkmanagertestfixture.h"

#include <catch2/catch.hpp>

using namespace std;

const wstring clientName = L"Test Client";

ClientWorkManagerTestFixture::ClientWorkManagerTestFixture()
  : manager(new ClientWorkManager(new Client(clientName)))
{
}

ClientWorkManagerTestFixture::~ClientWorkManagerTestFixture()
{
   delete manager;
}

void ClientWorkManagerTestFixture::CheckJobList(const Tools::WStringVec& expectedNames)
{
   vector<AbstractJob*> jobList;
   manager->GetJobList(jobList);

   const bool isSameSize = (expectedNames.size() == static_cast<int>(jobList.size()));
   if (isSameSize == false)
   {
      REQUIRE(jobList.size() == expectedNames.size());
      return;
   }

   auto itExpected = expectedNames.begin();
   auto itResult = jobList.begin();
   for (; itExpected != expectedNames.end(); ++itExpected, ++itResult)
      REQUIRE((*itResult)->GetName() == *itExpected);
}
