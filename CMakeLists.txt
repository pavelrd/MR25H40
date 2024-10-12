cmake_minimum_required(VERSION 3.5)

project(untitled2 LANGUAGES CXX)

# Выбор аппаратной платформы для которой компилировать программу
# Может быть либо PC либо SAM4E

set(HARDWARE_PLATFORM "PC")

if( ${HARDWARE_PLATFORM} MATCHES "SAM4E" )

    set(CMAKE_C_COMPILER arm-none-eabi-gcc.exe)
    set(CMAKE_CXX_COMPILER arm-none-eabi-g++.exe)

    # плюс добавить путь до линкера, пути до библиотек

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
#   - в switch не обрабатывается элемент перечисления
#   - в switch забыт break(implicit-fallthrough)
#

set(CMAKE_CXX_FLAGS "-Werror=all -Werror=return-type -Werror=float-equal -Werror=uninitialized -Werror=unused-variable -Werror=switch -Werror=implicit-fallthrough")

add_executable(untitled2 main.cpp spi.cpp io.hpp spi.hpp  test.cpp test.hpp checksum.hpp checksum.cpp)

add_subdirectory("mr25h40")

target_link_libraries(untitled2 PUBLIC libmr25h40)

add_subdirectory("port/PC")

add_subdirectory("port/SAM4E")

target_include_directories(libmr25h40 PUBLIC ${CMAKE_SOURCE_DIR}/mr25h40 )

if ( ${HARDWARE_PLATFORM} MATCHES "PC" )

    target_link_libraries(untitled2 PUBLIC libportpc)

elseif( ${HARDWARE_PLATFORM} MATCHES "SAM4E" )

    target_link_libraries(untitled2 PUBLIC libportsam4e)

endif()

find_package(Doxygen)
if (DOXYGEN_FOUND)
  set(DOXYGEN_GENERATE_HTML YES)
  set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/docs)
  doxygen_add_docs(doc ${CMAKE_SOURCE_DIR})
endif(DOXYGEN_FOUND)
