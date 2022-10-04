cmake_minimum_required(VERSION 3.13)
project(Question2)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall)


#include_directories(Utility)
#add_subdirectory(Utility)
#
#include_directories(FileParse)
#add_subdirectory(FileParse)

#need to link new package's makefile's lib file to the current executable name
add_executable(
        Question2
        main.cpp
        Utility/Record.h
        Utility/BlockListNode.h
        Utility/BytesOperation.h
        Utility/Record.cpp
        Utility/BlockListNode.cpp
        Utility/BytesOperation.cpp
        FileParse/ReadRawFile.h
        FileParse/ReadRawFile.cpp
        Utility/HashTableBlock.cpp
        Utility/HashTableBlock.h)
# target_link_libraries(Question2 Utility_lib FileParse_lib)