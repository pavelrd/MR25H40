cmake_minimum_required(VERSION 3.5)

set(SOURCE_LIB abstractmr25h40.hpp mr25h40.cpp mr25h40.hpp testmr25h40.cpp testmr25h40.hpp)

add_library(mr25h40 OBJECT ${SOURCE_LIB} )

target_include_directories(mr25h40 PUBLIC ${CMAKE_SOURCE_DIR} )
