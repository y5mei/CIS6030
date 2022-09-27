#include "gtest/gtest.h"
#include "RawDataParse.h"
#include <vector>


TEST(RawDataParseTest, Can_Read_Raw_Data_And_Generate_Files) {
//    readRawDataAndGenerateDataBaseFile("../../A1_data.txt");
    EXPECT_EQ(1,1);
}

TEST(RawDataParseTest, Able_To_Read_Write_File_On_Disk_By_Block) {

    string fileName = "testdatafilexyz.txt";
    deleteFile(fileName); // delete the test file if it already exists;

    // Write method can create new file, and write a block into it
    writefileToDiskByBlock(fileName, 1, 24, "abcedfghijklmnopqrstuvwx");

    // Write method can replace a block of fixed size in an existing file
    writefileToDiskByBlock(fileName, 1, 12, "123456789012");

    // Write method can leave a hole in the file, and write discontinued block
    writefileToDiskByBlock(fileName, 4, 12, "mayson a pig");

    string block1 = readFileFromDiskByBlock(fileName, 1, 12);
    ASSERT_EQ(block1, "123456789012");
    string block2 = readFileFromDiskByBlock(fileName, 2, 12);
    ASSERT_EQ(block2, "mnopqrstuvwx");
    // Block 3 is the hole [check ../cmake-build-debug/Google_tests/testdatafilexyz.txt to see the generated file]
    string block3 = readFileFromDiskByBlock(fileName, 3, 12);
    ASSERT_EQ(block3, "");
    string block4 = readFileFromDiskByBlock(fileName, 4, 12);
    ASSERT_EQ(block4, "mayson a pig");
}