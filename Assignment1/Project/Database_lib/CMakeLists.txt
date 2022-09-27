project(Database_lib)

set(HEADER_FILES
        DatabaseFileIO.h
        RawDataParse.h
        )

set(SOURCE_FILES
        DatabaseFileIO.cpp
        RawDataParse.cpp
        )

add_library(Database_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})