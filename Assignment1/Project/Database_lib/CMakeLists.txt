project(Database_lib)

set(HEADER_FILES
        DatabaseFileIO.h
        )

set(SOURCE_FILES
        DatabaseFileIO.cpp
        )

add_library(Database_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})