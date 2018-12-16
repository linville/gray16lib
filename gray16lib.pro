#
# Gray16Lib master qmake file.
#

!versionAtLeast(QT_VERSION, 5.13.0) {
  message("Need Qt 5.13 for QImage::Format_Grayscale16 support. See QTBUG-41176")
}

TEMPLATE = subdirs

SUBDIRS += gray16demo \
           gray16lib \
           unit_tests

gray16demo.subdir = gray16demo
gray16lib.subdir = gray16lib
unit_tests.subdir = unit_tests

gray16demo.depends = gray16lib
unit_tests.depends = gray16lib
