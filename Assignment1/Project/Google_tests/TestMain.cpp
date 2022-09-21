#include "gtest/gtest.h"
#include "Gregorian.h"

TEST(a, b) {
    short a = 1;
    short b = 1;
    ASSERT_EQ(a, b);
}

TEST(AbsoluteDateTestSuite, ExampleDate) { // 12/2/2020 -> 737761
    GregorianDate gregDate;
    gregDate.SetMonth(12);
    gregDate.SetDay(2);
    gregDate.SetYear(2020);

    EXPECT_EQ(gregDate.getAbsoluteDate(), 737761);
}


int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}