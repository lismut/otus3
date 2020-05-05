#include <gtest/gtest.h>
#include "lib.h"

TEST(GOOGLE_TEST1, test2)
{
	ASSERT_TRUE(version() > 0);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
