project(FileParse_lib)

set(HEADER_FILES
        ReadRawFile.h
        )

set(SOURCE_FILES
        ReadRawFile.cpp
        )

add_library(FileParse_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})