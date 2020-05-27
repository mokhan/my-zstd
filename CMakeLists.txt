cmake_minimum_required(VERSION 2.8.11)
project(example C)

set(CMAKE_VERBOSE_MAKEFILE FALSE)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(${PROJECT_NAME} example.c)
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
