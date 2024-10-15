cmake_minimum_required(VERSION 3.5)

if( ${HARDWARE_PLATFORM} MATCHES "SAM4E" )

    set(SOURCE_LIB ioSAM4E.cpp spiSAM4E.cpp)

    add_library(portsam4e OBJECT ${SOURCE_LIB} )

    target_include_directories(portsam4e PUBLIC ${CMAKE_SOURCE_DIR} )

endif()
