cmake_minimum_required(VERSION 3.5)

set(SOURCE_LIB test_workers.cpp tests.cpp tests.hpp checksum.cpp checksum.hpp)

add_library(tests OBJECT ${SOURCE_LIB} )

target_include_directories(tests PUBLIC ${CMAKE_SOURCE_DIR} )

target_include_directories(tests PUBLIC ${CMAKE_SOURCE_DIR}/mr25h40 )

#target_include_directories(tests PUBLIC ${CMAKE_SOURCE_DIR}/googletest )
