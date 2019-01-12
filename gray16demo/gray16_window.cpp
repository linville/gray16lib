/*!
  \file gray16demo/gray16_window.cpp
  \brief Implementation of the Gray16Window class.
*/

#include "gray16_window.h"

#include "image_histogram_widget.h"
#include "offset_gain_options_widget.h"
#include "manual_options_widget.h"

#include <QtConcurrent>

/*!
  \brief Constructor for the Gray16Window
*/

Gray16Window::Gray16Window(QWidget *parent)
  : QMainWindow(parent) {
  setupUi(this);

  connect(fileOpenAction, &QAction::triggered,
          this, &Gray16Window::onFileOpen);
  connect(fileExitAction, &QAction::triggered,
          this, &Gray16Window::close);
  connect(helpAboutAction, &QAction::triggered,
          this, &Gray16Window::onHelpAbout);
  connect(helpAboutQtAction, &QAction::triggered,
          qApp, &QApplication::aboutQt);

  connect(standardImageLabel, &TrackingLabel::imageCoordinate,
          this, &Gray16Window::mUpdateValue);
  connect(advancedImageLabel, &TrackingLabel::imageCoordinate,
          this, &Gray16Window::mUpdateValue);

  standardImageLabel->setOtherLabel(advancedImageLabel);
  advancedImageLabel->setOtherLabel(standardImageLabel);

  connect(conversionCombo, QOverload<const QString &>::of(&QComboBox::activated),
          this, &Gray16Window::onConversionCombo);

  mSetupExamples();
  mSetupWidgets();

  QFont dataFont("Monospace");
  dataFont.setFixedPitch(true);
  dataFont.setStyleHint(QFont::Monospace);
  originalValueEdit->setFont(dataFont);
  convertedValueEdit->setFont(dataFont);

  QSettings settings;
  restoreGeometry(settings.value("Gray16Window/Geometry").toByteArray());
}

void Gray16Window::onFileOpen() {
  QSettings settings;

  const auto defaultPath =
      settings.value("Gray16Window/DefaultPath",
                     QStandardPaths::standardLocations(QStandardPaths::PicturesLocation)).toString();

  const QString fileName =
      QFileDialog::getOpenFileName(this, "Open 16-bit PNG",
                                   defaultPath,
                                   tr("Image Files (*.png)"));

  if(fileName.isEmpty()) {
    return;
  }

  QFileInfo fileInfo(fileName);
  settings.setValue("Gray16Window/DefaultPath", fileInfo.absoluteDir().absolutePath());

  openFile(fileName);
}

void Gray16Window::onHelpAbout() {
  QMessageBox::about(this, "Gray16 Demonstration GUI",
                     "Demonstrating 16-bit Grayscale Image Processing.");
}

void Gray16Window::openFile(const QString &imagePath) {
  if(!mOriginalImage.load(imagePath)) {
    qCritical() << "Couldn't load image:" << imagePath;
    return;
  }

  if(mOriginalImage.format() != QImage::Format_Grayscale16) {
    QMessageBox::critical(this, "Not 16-bit Grayscale",
                          "This selecetd image was not 16-bit grayscale.");
  }

  standardImageLabel->setPixmap(QPixmap::fromImage(mOriginalImage));
  mConvert(conversionOptionsStack->currentIndex());
}

void Gray16Window::onConversionCombo(const QString &text) {
  if(text == "Manual") {
    conversionOptionsStack->setCurrentIndex(0);
    mConvert(0);
  } else if(text == "False Color") {
    conversionOptionsStack->setCurrentIndex(1);
    mConvert(1);
  } else if(text == "Neighbor Scale") {
    conversionOptionsStack->setCurrentIndex(2);
    mConvert(2);
  } else if(text == "Offset / Gain") {
    conversionOptionsStack->setCurrentIndex(3);
    mConvert(3);
  } else {
    qCritical() << "Unhandled mode:" << text;
  }
}

void Gray16Window::closeEvent(QCloseEvent *e) {
  QSettings settings;
  settings.setValue("Gray16Window/Geometry", saveGeometry());

  e->accept();
}

void Gray16Window::mConvert(int index) {
  if(mOriginalImage.isNull()) {
    return;
  }

  imageHistogramWidget->setImage(mOriginalImage);

  if(index == 0) {
    mConvertedImage = mManualConvert.convert(mOriginalImage);
  } else if(index == 1) {
    mConvertedImage = mFalseColorConvert.convert(mOriginalImage);
  } else if(index == 2) {
    QProgressDialog dialog;
    dialog.setWindowTitle("Neighbor Scale");
    dialog.setLabelText("Converting...");

    QFutureWatcher<QImage> futureWatcher;
    connect(&futureWatcher, SIGNAL(finished()), &dialog, SLOT(reset()));

    futureWatcher.setFuture(
          QtConcurrent::run(mNeighborScaleConvert,
                            static_cast<QImage (Gray16::NeighborScale::*)(const QImage &) const>(&Gray16::NeighborScale::convert),
                            mOriginalImage)
          );

    dialog.exec();
    futureWatcher.waitForFinished();

    mConvertedImage = futureWatcher.result();
  } else if(index == 3) {
    mConvertedImage = mOffsetGainConvert.convert(mOriginalImage);
    mOffsetGainOptionsWidget->updateOffset(mOffsetGainConvert.detectedOffset());
  } else {
    qCritical() << "Unhandled conversion:" << index;
  }

  advancedImageLabel->setPixmap(QPixmap::fromImage(mConvertedImage));
}

void Gray16Window::mUpdateValue(int x, int y) {
  uint16_t originalValue = reinterpret_cast<uint16_t *> (mOriginalImage.scanLine(y))[x];
  originalValueEdit->setText(QString::number(originalValue));

  imageHistogramWidget->highlightValue(originalValue);

  if(mConvertedImage.format() == QImage::Format_Grayscale8) {
    uint8_t convertedValue = mConvertedImage.scanLine(y)[x];
    convertedValueEdit->setText(QString::number(convertedValue));
  } else if(mConvertedImage.format() == QImage::Format_RGB32
            || mConvertedImage.format() == QImage::Format_ARGB32) {
    uint32_t convertedValue = reinterpret_cast<uint32_t *> (mConvertedImage.scanLine(y))[x];

    const int r = (convertedValue & 0xFF0000) >> 16;
    const int g = (convertedValue & 0x00FF00) >> 8;
    const int b = (convertedValue & 0x0000FF);

    const auto tmp = QString("R:%1, G:%2, B:%3")
        .arg(r, 3, 10, QChar('0'))
        .arg(g, 3, 10, QChar('0'))
        .arg(b, 3, 10, QChar('0'));

    convertedValueEdit->setText(tmp);
  }
}

void Gray16Window::mSetupExamples() {
  menuExamples->addAction("Example 1 - Lazy Dog", [this]() {
    openFile(":examples/example-1-lazy-dog.png");
  });

  menuExamples->addAction("Example 2 - LED Floodlight", [this]() {
    openFile(":examples/example-2-led-floodlight.png");
  });
}

void Gray16Window::mSetupWidgets() {
  // Manual Converter (always first, everything else alphabetical)
  connect(mManualOptionsWidget, &ManualOptionsWidget::optionsUpdated, this,
          [this]() {
    mConvert(0);
  });
  conversionOptionsStack->addWidget(mManualOptionsWidget);
  conversionCombo->addItem("Manual");

  // False Color Converter
  connect(mFalseColorOptionsWidget, &FalseColorOptionsWidget::optionsUpdated, this,
          [this]() {
    mConvert(1);
  });
  conversionOptionsStack->addWidget(mFalseColorOptionsWidget);
  conversionCombo->addItem("False Color");

  // Neighbor Scale Converter
  connect(mNeighborScaleOptionsWidget, &NeighborScaleOptionsWidget::optionsUpdated, this,
          [this]() {
    mConvert(2);
  });
  conversionOptionsStack->addWidget(mNeighborScaleOptionsWidget);
  conversionCombo->addItem("Neighbor Scale");

  // Offset / Gain Converter
  connect(mOffsetGainOptionsWidget, &OffsetGainOptionsWidget::optionsUpdated, this,
          [this]() {
    mConvert(3);
  });
  conversionOptionsStack->addWidget(mOffsetGainOptionsWidget);
  conversionCombo->addItem("Offset / Gain");
}
