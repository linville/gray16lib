# Top-level Gray16Lib CMake Build File
cmake_minimum_required(VERSION 3.12)

project("Gray16Lib Project" LANGUAGES CXX)

enable_testing()

add_subdirectory(gray16lib)
add_subdirectory(gray16demo)
add_subdirectory(unit_tests/test_lib)