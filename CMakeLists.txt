cmake_minimum_required(VERSION 3.5)

project(containers)

set(CMAKE_CXX_STANDARD 14)

set (THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/third_party)

add_compile_options(-Wall -Wextra -Wpedantic)

add_subdirectory(containers)
