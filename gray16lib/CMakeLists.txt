# Gray16Lib Library Build File
cmake_minimum_required(VERSION 3.12)

project(Gray16Lib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Gui)

add_library(Gray16Lib OBJECT
  converter_base.cpp
  false_color_convert.cpp
  manual_convert.cpp
  neighbor_scale.cpp
  offset_gain_convert.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Gui)
