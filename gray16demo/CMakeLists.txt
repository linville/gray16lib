# Top-level Gray16Lib CMake Build File
cmake_minimum_required(VERSION 3.12)

project(Gray16Demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Concurrent)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTOUIC_SEARCH_PATHS conversion_widgets)

add_executable(${PROJECT_NAME} MACOSX_BUNDLE
  gray16_window_base.ui
  
  conversion_widgets/false_color_options_widget.h
  conversion_widgets/manual_options_widget.h
  conversion_widgets/neighbor_scale_options_widget.h
  conversion_widgets/offset_gain_options_widget.h
  
  conversion_widgets/false_color_options_widget_base.ui
  conversion_widgets/manual_options_widget_base.ui
  conversion_widgets/neighbor_scale_options_widget_base.ui
  conversion_widgets/offset_gain_options_widget_base.ui

  resources.qrc
  
  main.cpp
  gray16_window.cpp
  image_histogram_widget.cpp
  tracking_label.h

  $<TARGET_OBJECTS:Gray16Lib>
)

target_compile_definitions(${PROJECT_NAME} PUBLIC APP_VERSION="0.0.0")

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/gray16lib)
target_include_directories(${PROJECT_NAME} PUBLIC conversion_widgets)

target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Concurrent)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Widgets)
