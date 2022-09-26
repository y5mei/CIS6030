#include "gtest/gtest.h"
#include "DatabaseFileIO.h"

// Record constructor works with str with more than 3 spaces.
TEST(Database_lib, Record_Can_Be_Constructed_From_Raw_String) {
    Record r = Record("123456789 field 2 something");
    ASSERT_EQ(r.field1, "123456789");
    ASSERT_EQ(r.field2, "field 2");
}

TEST(SubString, SubStr_Is_Working_For_Extra_Index) {
    std::string str = "123";
    std::string f1 = str.substr(0, 3);
    std::string f2 = str.substr(3, -900); // if pos out of index, no matter what n is, substr is ""
    std::string f3 = str.substr(3);
    ASSERT_EQ(f1, str);
    ASSERT_EQ(f2, "");
    ASSERT_EQ(f3, "");
}

TEST(Database_lib, Record_Should_Throw_If_Field1_Less_Than_9_Bytes) {
    EXPECT_THROW({
                     try {
                         Record r = Record("12345678 field 2");
                     }
                     catch (const std::invalid_argument &e) {
                         // and this tests that it has the correct message
                         EXPECT_STREQ("The length of first field must be 9 bytes!", e.what());
                         throw;
                     }
                 }, std::invalid_argument);
}

TEST(Database_lib, Record_Should_Have_Valid_Field_Two) {
    // Must have two spaces to have both field 1 and field 2;
    EXPECT_THROW({
                     try {
                         Record r = Record("123456789 field");
                     }
                     catch (const std::invalid_argument &e) {
                         // and this tests that it has the correct message
                         EXPECT_STREQ("Input str is invalid, you need at least 3 spaces in the str.", e.what());
                         throw;
                     }
                 }, std::invalid_argument);

    // But Field3 is optional
    Record r2 = Record("123456789 f 2");
    ASSERT_EQ(r2.field1, "123456789");
    ASSERT_EQ(r2.field2, "f 2");
    ASSERT_EQ(r2.field3, "");

    Record r3 = Record("123456789 f 2 ");
    ASSERT_EQ(r3.field1, "123456789");
    ASSERT_EQ(r3.field2, "f 2");
    ASSERT_EQ(r3.field3, "");

    Record r4 = Record("123456789 f 2 field3 more field3");
    ASSERT_EQ(r4.field1, "123456789");
    ASSERT_EQ(r4.field2, "f 2");
    ASSERT_EQ(r4.field3, "field3 more field3");
}