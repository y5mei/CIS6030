cmake_minimum_required(VERSION 3.13)
project(Project)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp)
add_executable(Database_run ${SOURCE_FILES})
#add_executable(Project main.cpp) # this is not for unit test

#For unit test
include_directories(DateConverter_lib)
add_subdirectory(DateConverter_lib)

include_directories(Database_lib)
add_subdirectory(Database_lib)

target_link_libraries(Database_run Database_lib)
add_subdirectory(Google_tests)