/*!
  \file gray16demo/main.cpp
  \brief main(..) for the Gray16-Convert Demonstration application.
*/

#include "gray16_window.h"

#include <QtCore>
#include <QtGui>

int main(int argc, char *argv[]) {
  QApplication myApp(argc, argv);

  QCoreApplication::setOrganizationName("Aaron Linville");
  QCoreApplication::setOrganizationDomain("linville.org");
  QCoreApplication::setApplicationName("Gray16");
  QCoreApplication::setApplicationVersion(APP_VERSION);

  Gray16Window mainWindow;
  mainWindow.show();

  return myApp.exec();
}
