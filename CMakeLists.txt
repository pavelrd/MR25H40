cmake_minimum_required(VERSION 3.5)

project(untitled2 LANGUAGES CXX)

# Выбор аппаратной платформы для которой компилировать программу
# Может быть либо PC  либо SAM4E

set(HARDWARE_PLATFORM "PC")

if( ${HARDWARE_PLATFORM} MATCHES "SAM4E" )

    set(CMAKE_C_COMPILER arm-none-eabi-gcc.exe)
    set(CMAKE_CXX_COMPILER arm-none-eabi-g++.exe)

endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Дополнительные флаги для более строгой компиляции
#
#  Они не позволят скомпилировать программу если:
#
#   - забыт return
#   - прямое сравнение float, типа ==
#   - есть неинициализированная или неиспользуемая переменная
#   - в конструкции switch забыт break
#

set(CMAKE_CXX_FLAGS "-Werror=all -Werror=return-type -Werror=float-equal -Werror=uninitialized -Werror=unused-variable -Werror=switch -Werror=implicit-fallthrough")

add_executable(untitled2 main.cpp mr25h40.cpp spi.cpp io.hpp mr25h40.hpp spi.hpp abstractmr25h40.hpp testmr25h40.hpp testmr25h40.cpp test.cpp test.hpp checksum.hpp checksum.cpp)

add_subdirectory("port/PC")

add_subdirectory("port/SAM4E")

if ( ${HARDWARE_PLATFORM} MATCHES "PC" )

    target_link_libraries(untitled2 PUBLIC libportpc)

elseif( ${HARDWARE_PLATFORM} MATCHES "SAM4E" )

    target_link_libraries(untitled2 PUBLIC libportsam4e)

endif()
