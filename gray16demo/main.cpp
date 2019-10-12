/*!
  \file gray16demo/main.cpp
  \brief main(..) for the Gray16Demo Demonstration application.
*/

#include "gray16_window.h"

#include <QtCore>
#include <QtGui>

/*!
  \brief Gray16Demo main(..)

  Optionally, a path to a PNG image can be passed as a single positional
  argument and it will be opened at startup.
*/

int main(int argc, char *argv[]) {
  QApplication myApp(argc, argv);

  QCoreApplication::setOrganizationName("Aaron Linville");
  QCoreApplication::setOrganizationDomain("linville.org");
  QCoreApplication::setApplicationName("Gray16");
  QCoreApplication::setApplicationVersion(APP_VERSION);

  QCommandLineParser parser;
  parser.setApplicationDescription("16-bit Grayscale Processing");
  parser.addHelpOption();
  parser.addVersionOption();
  parser.addPositionalArgument("file", "png");

  parser.parse(QApplication::arguments());

  Gray16Window mainWindow;
  mainWindow.show();

  const auto args = parser.positionalArguments();
  if(!args.empty()) {
    const auto &filePath = args.first();
    if(!QFileInfo::exists(filePath)) {
      qCritical() << "Can't open" << filePath;
      return -1;
    }

    if(!filePath.endsWith(".png", Qt::CaseInsensitive)) {
      qCritical() << "Doesn't appear to be a PNG.";
      return -1;
    }

    mainWindow.openFile(filePath);
  }

  return myApp.exec();
}
