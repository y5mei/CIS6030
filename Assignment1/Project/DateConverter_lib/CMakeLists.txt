project(DateConverter)

set(HEADER_FILES
        Gregorian.h
        )

set(SOURCE_FILES
        Gregorian.cpp
        )

add_library(DateConverter_libxxx STATIC ${SOURCE_FILES} ${HEADER_FILES})