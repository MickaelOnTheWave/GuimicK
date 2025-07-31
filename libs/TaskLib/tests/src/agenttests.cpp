#include <gtest/gtest.h>

#include "SelfIdentity.h"

using namespace std;

//-------------------------------------------------------------------------

namespace  {

}

//-------------------------------------------------------------------------

TEST(Agent, Error_NonexistentFile)
{
   EXPECT_EQ(10, 10);
}

TEST(Agent, Error_MissingCloseBrace)
{
   EXPECT_EQ(10, 11);
}

