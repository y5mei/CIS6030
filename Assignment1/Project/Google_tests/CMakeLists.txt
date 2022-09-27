project(Google_tests)
add_subdirectory(lib)
#include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
#unit test
include_directories(lib/googletest-main/googletest/include)
include_directories(lib/googletest-main/googlemock/include)
include_directories(../Database_lib)
include_directories(../DateConverter_lib/)
include_directories(../BPlusTree_lib/)

# adding the Google_Tests_run target
add_executable(Google_tests TestMain.cpp RecordTests.cpp BPlusTreeTests.cpp BlockTests.cpp RawDataParseTests.cpp)

# linking Google_Tests_run with DateConverter_lib which will be tested
target_link_libraries(Google_tests DateConverter_libxxx)
target_link_libraries(Google_tests Database_lib BPlusTree_lib)
target_link_libraries(Google_tests gtest gtest_main)
target_link_libraries(Google_tests gmock gmock_main)