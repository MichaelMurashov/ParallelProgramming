cmake_minimum_required(VERSION 3.6)
project(level_set)

set(CMAKE_CXX_STANDARD 11)

set(LIBRARY "_level_set")
add_subdirectory(src)

set(SOURCE_FILES main.cpp)
add_executable(level_set ${SOURCE_FILES})

target_link_libraries(level_set &{LIBRARY})
