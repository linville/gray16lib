/*!
  \file gray16lib_tests.cpp
  \brief Gray16Lib Unit Tests.
*/

#include <QtTest/QtTest>

#include "gray16lib.h"

#include <algorithm>

/*!
  \class Gray16LibTests
  \brief Tests for the various geometry algorithms
*/

class Gray16LibTests : public QObject {
  Q_OBJECT

private slots:
  void initTestCase();

  void neighborScale();
  void neighborScale_data();
  
private:
  QStringList mExamples() const;

  QString mExamplesPath;
};

void Gray16LibTests::initTestCase() {
  QStringList paths = {
    "../gray16demo/examples",
    "gray16demo/examples",
    "../../gray16demo/examples"
  };

  const auto fileName = mExamples().constFirst();

  if(fileName == mExamplesPath.constEnd()) {
    QSKIP("Examples list was empty.");
  }

  const auto iter = std::find_if(paths.constBegin(),
                                 paths.constEnd(),
                                 [&fileName](const QString &path) {
    return QFileInfo::exists(QString("%1/%2").arg(path, fileName));
  });

  if(iter->isEmpty()) {
    QSKIP("Couldn't find examples.");
  }

  mExamplesPath = *iter;
}

void Gray16LibTests::neighborScale() {
  QFETCH(QString, filename);
  
  QImage original(QString("%1/%2").arg(mExamplesPath, filename));
  
  Gray16::NeighborScale converter;
  converter.setCompareDistance(4);
  
  QImage convertedImage;
  QBENCHMARK {
    convertedImage = converter.convert(original);
  }
  
  QVERIFY(!convertedImage.isNull());
}

void Gray16LibTests::neighborScale_data() {
  QTest::addColumn<QString>("filename");
  
  for(const auto &example : mExamples()) {
    QTest::newRow(example.toLatin1().constData()) << example;
  }
}

QStringList Gray16LibTests::mExamples() const {
  return {
    "example-1-lazy-dog.png",
    "example-2-led-floodlight.png"
  };
}

QTEST_MAIN(Gray16LibTests)
#include "gray16lib_tests.moc"
