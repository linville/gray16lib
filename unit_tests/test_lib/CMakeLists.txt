# Gray16Lib Unit Tests
cmake_minimum_required(VERSION 3.12)

project(test_lib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Gui)
find_package(Qt6 REQUIRED COMPONENTS Test)

add_executable(${PROJECT_NAME}
  gray16lib_tests.cpp

  $<TARGET_OBJECTS:Gray16Lib>
)

add_test(NAME ${PROJECT_NAME}
  COMMAND ${PROJECT_NAME}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/gray16lib)

target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Gui)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Test)
