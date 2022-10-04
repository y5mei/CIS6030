project(Utility_lib)

set(HEADER_FILES
        BytesOperation.h
         Record.h
        BlockListNode.h)

set(SOURCE_FILES
        BytesOperation.cpp
        Record.cpp BlockListNode.cpp)

add_library(Utility_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})