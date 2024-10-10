cmake_minimum_required(VERSION 3.5)

set(SOURCE_LIB ioPC.cpp spiPC.cpp)

add_library(libportpc OBJECT ${SOURCE_LIB} )

target_include_directories(libportpc PUBLIC ${CMAKE_SOURCE_DIR} )
