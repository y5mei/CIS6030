#include "gtest/gtest.h"
#include "RawDataParse.h"
#include <vector>
#include <bitset>


TEST(RawDataParseTest, Short_Char__String_RoundTrip) {
    // Round trip test to cast short to two bytes, and cast two shorts into a string of four bytes.
    // Estimation num of Tree node: 15K, Block: 3K, record less than 100;
    // Didn't test, but this should not work with negative short numbers
//    for (short i = 0; i < 20000; ++i) {
//        CharShort a = CharShort(i);
//        CharShort b = CharShort(a.high, a.low);
//        CharShort c = CharShort(b.num);
//        ASSERT_EQ(a.high, c.high);
//        ASSERT_EQ(a.low, c.low);
//    }
//
//    for (short i = 0; i < 4000; ++i) { // block num around 3300
//        for (short j = 0; j < 100; ++j) { // record num around 25
//            StingShort a = StingShort(i, j);
//            StingShort b = StingShort(a.str);
//            StingShort c = StingShort(b.block, b.record);
//            ASSERT_EQ(a.str, c.str);
//        }
//    }
}

TEST(RawDataParseTest, Can_Read_Raw_Data_And_Generate_Files) {
    readRawDataAndGenerateDataBaseFile("../../A1_data.txt");
//    EXPECT_EQ(1, 1);
}

TEST(RawDataParseTest, Search_Directly_From_Disk) {
// A random key that exist
//    vector<Record> records = readRawTxtFile("../../A1_data.txt");
//    int n = 1;
//    while (n > 0) {
//        srand((unsigned int) time(NULL));
//        int random = rand() % records.size();
//        cout << random << endl;
//        Record r = records[random];
//        ASSERT_EQ(r.field1, search(r.field1));
//        n--;
//    }
//    // key does not exist
//    search("123456789");
//
//    // Search the first key
//    search("aaagbmhha");
//
//    // Search the last key
//    search("zzzjzagrk");

    search("xeuudlpdt");
    search("xexzrbtpx");
}

TEST(RawDataParseTest, Insert_A_New_Record){
//    search("xeustugyv");
//    insert("xeustugyw long string a                                                                              \n"
//           "                                                                                                          \n"
//           "                                                                                          very long string");
    insert("xeustugyz mayson is a very cute pig!");
//    search("xeustugyw");
//    search("xeustugyz");
}

TEST(RawDataParseTest, Search_After_Insert){
    search("xeustugyz");
}

TEST(RawDataParseTest, Able_To_Read_Write_File_On_Disk_By_Block) {

    string fileName = "testdatafilexyz.txt";
    deleteFile(fileName); // delete the test file if it already exists;

    // Write method can create new file, and write a block into it
    writefileToDiskByBlock(fileName, 1, 24, "abcedfghijklmnopqrstuvwx");

    // Write method can replace a block of fixed size in an existing file
    writefileToDiskByBlock(fileName, 1, 12, "123456789012");

    // Write method can leave a hole in the file, and write discontinued block
    char mychar[12];
    for (int i = 0; i < 12; ++i) {
        mychar[i] = '0' + i;
    }
    writefileToDiskByBlock(fileName, 4, 12, mychar);

    string block1 = readFileFromDiskByBlock(fileName, 1, 12);
    ASSERT_EQ(block1, "123456789012");
    string block2 = readFileFromDiskByBlock(fileName, 2, 12);
    ASSERT_EQ(block2, "mnopqrstuvwx");
    // Block 3 is the hole [check ../cmake-build-debug/Google_tests/testdatafilexyz.txt to see the generated file]
    string block3 = readFileFromDiskByBlock(fileName, 3, 12);
    ASSERT_EQ(block3[0], '\0');
    ASSERT_EQ(block3[11], '\0');
    string block4 = readFileFromDiskByBlock(fileName, 4, 12);
    ASSERT_EQ(block4, "0123456789:;");
}