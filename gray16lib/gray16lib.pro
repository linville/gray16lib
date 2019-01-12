#
# Gray16Lib - Library of Gray16 image processing algorithms.
#

TARGET = Gray16Lib
TEMPLATE = lib

QT += core gui concurrent

CONFIG += staticlib
CONFIG += c++11 c++14 c++17 warn_on
CONFIG += debug_and_release

SOURCES  +=                \
    converter_base.cpp     \
    false_color_convert.cpp \
    manual_convert.cpp     \
    neighbor_scale.cpp     \
    offset_gain_convert.cpp

HEADERS  +=                \
    gray16lib.h            \
    converter_base.h       \
    false_color_convert.h  \
    manual_convert.h       \
    neighbor_scale.h       \
    offset_gain_convert.h

PROJ_ROOT = ..
include($${PROJ_ROOT}/common.pri)
