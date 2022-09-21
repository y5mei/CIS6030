project(BPlusTree_lib)

set(HEADER_FILES
        BPlusTree.h
        )

set(SOURCE_FILES
        BPlusTree.cpp
        )

add_library(BPlusTree_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})