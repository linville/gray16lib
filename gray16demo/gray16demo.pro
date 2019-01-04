#
# Gray16 Demonstration GUI
#

TARGET = Gray16Demo
TEMPLATE = app

QT += core gui widgets

CONFIG += c++11 c++14 c++17 warn_on
CONFIG += debug_and_release

SOURCES  +=                \
    main.cpp               \
    gray16_window.cpp      \
    image_histogram_widget.cpp

HEADERS  +=                \
    gray16_window.h        \
    image_histogram_widget.h \
    tracking_label.h       \
    conversion_widgets/false_color_options_widget.h \
    conversion_widgets/manual_options_widget.h \
    conversion_widgets/offset_gain_options_widget.h

INCLUDEPATH +=             \
    conversion_widgets

FORMS    +=                \
    gray16_window_base.ui  \
    conversion_widgets/false_color_options_widget_base.ui \
    conversion_widgets/manual_options_widget_base.ui \
    conversion_widgets/offset_gain_options_widget_base.ui

RESOURCES += resources.qrc

ICON = resources/Gray16Demo.icns
RC_ICONS = resources/Gray16Demo.ico

macx: QMAKE_INFO_PLIST = resources/Info.plist

PROJ_ROOT = ..
include($${PROJ_ROOT}/common.pri)

INCLUDEPATH += $${PROJ_ROOT}/gray16lib
win32: PRE_TARGETDEPS += $${DESTDIR}/Gray16Lib$${BUILD_SUFFIX}.lib
unix: PRE_TARGETDEPS += $${DESTDIR}/libGray16Lib$${BUILD_SUFFIX}.a
LIBS += -L$${DESTDIR} -lGray16Lib$${BUILD_SUFFIX}
