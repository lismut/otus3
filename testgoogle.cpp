#include <gtest/gtest.h>
#include "lib.h"

TEST(GOOGLE_TEST1, test2)
{
	ASSERT_TRUE(version() > 1);
}