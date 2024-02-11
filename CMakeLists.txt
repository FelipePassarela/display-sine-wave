cmake_minimum_required(VERSION 3.12)
project(display-sine-wave)

set(CMAKE_CXX_STANDARD 17)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(display-sine-wave-debug ${SOURCES})
add_executable(display-sine-wave ${SOURCES})

target_compile_options(display-sine-wave-debug PRIVATE -Wall -Wextra -Werror -g -O0 -DDEBUG)
target_compile_options(display-sine-wave PRIVATE -O3 -DNDEBUG)
