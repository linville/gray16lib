#
# Common items for all the subdir projects
#

VERSION = 1
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

PROJ_ROOT = $${PWD}
DESTDIR = $${PROJ_ROOT}/bin

build_pass:CONFIG(debug, debug|release) {
  !macx:!win32: BUILD_SUFFIX = "_debug"
  macx: BUILD_SUFFIX = " Debug"
  win32: BUILD_SUFFIX = "Debug"
  
  TARGET = $$join(TARGET,,,$${BUILD_SUFFIX})
  CONFIG += console
}

OBJECTS_DIR = $${PROJ_ROOT}/tmp/$${TARGET}/obj
MOC_DIR = $${PROJ_ROOT}/tmp/$${TARGET}/moc
UI_DIR = $${PROJ_ROOT}/tmp/$${TARGET}/uic
RCC_DIR = $${PROJ_ROOT}/tmp/$${TARGET}/rcc
