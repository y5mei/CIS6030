project(Utility_lib)

set(HEADER_FILES
        BytesOperation.h
        Record.cpp Record.h BlockListNode.cpp BlockListNode.h)

set(SOURCE_FILES
        BytesOperation.cpp
        Record.cpp Record.h BlockListNode.cpp BlockListNode.h)

add_library(Utility_lib STATIC ${SOURCE_FILES} ${HEADER_FILES} Record.cpp Record.h BlockListNode.cpp BlockListNode.h)