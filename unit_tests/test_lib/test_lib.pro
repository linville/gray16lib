# Gray16Lib Unit Tests

TARGET = test_gray16lib

QT += testlib

CONFIG += release warn_on cmdline testcase

SOURCES += gray16lib_tests.cpp

PROJ_ROOT = ../..
include($${PROJ_ROOT}/common.pri)

INCLUDEPATH += $${PROJ_ROOT}/gray16lib
win32: PRE_TARGETDEPS += $${DESTDIR}/Gray16Lib$${BUILD_SUFFIX}.lib
unix: PRE_TARGETDEPS += $${DESTDIR}/libGray16Lib$${BUILD_SUFFIX}.a
LIBS += -L$${DESTDIR} -lGray16Lib$${BUILD_SUFFIX}
