project(Database_lib)

set(HEADER_FILES
        DatabaseFileIO.h
        RawDataParse.h
        FinalPrintOut.h
        )

set(SOURCE_FILES
        DatabaseFileIO.cpp
        RawDataParse.cpp
        FinalPrintOut.cpp)

add_library(Database_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})