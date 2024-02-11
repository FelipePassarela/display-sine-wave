cmake_minimum_required(VERSION 3.12)
project(display-funcao-seno)

set(CMAKE_CXX_STANDARD 17)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(display-funcao-seno ${SOURCES})

target_compile_options(display-funcao-seno PRIVATE -Wall -Wextra -Werror -g -DDEBUG)
