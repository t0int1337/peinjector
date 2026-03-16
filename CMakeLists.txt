cmake_minimum_required(VERSION 3.20)
project(untitled)

set(CMAKE_CXX_STANDARD 20)

find_package(LIEF REQUIRED)

add_executable(untitled main.cpp)

target_link_libraries(untitled PRIVATE LIEF::LIEF)