/*!
  \file gray16demo/gray16_window.h
  \brief Definition of Gray16Window class.

  UNCLASSIFIED
*/

#ifndef GRAY16_WINDOW_H
#define GRAY16_WINDOW_H

#include "ui_gray16_window_base.h"

#include "false_color_options_widget.h"
#include "manual_options_widget.h"
#include "offset_gain_options_widget.h"

#include "gray16lib.h"
#include "image_histogram_widget.h"

#include <QtWidgets>

/*!
  \brief Gray16Window - Main window for 16-bit to 8-bit grayscale conversion.
*/

class Gray16Window : public QMainWindow, private Ui::Gray16WindowBase {
  Q_OBJECT
public:
  explicit Gray16Window(QWidget *parent = nullptr);

public slots:
  void onFileOpen();
  void onHelpAbout();

  void openFile(const QString &imagePath);
  void onConversionCombo(const QString &text);

protected:
  void closeEvent(QCloseEvent *e);

private:
  void mSetupExamples();
  void mSetupWidgets();

  void mConvert(int index);
  void mUpdateValue(int x, int y);

  QImage mOriginalImage; //!< Original 16-bit grayscale image
  QImage mConvertedImage; //!< Converted image

  Gray16::FalseColorConvert mFalseColorConvert;
  Gray16::ManualConvert mManualConvert;
  Gray16::OffsetGainConvert mOffsetGainConvert;

  FalseColorOptionsWidget *mFalseColorOptionsWidget =
      new FalseColorOptionsWidget(&mFalseColorConvert);
  ManualOptionsWidget *mManualOptionsWidget =
      new ManualOptionsWidget(&mManualConvert);
  OffsetGainOptionsWidget *mOffsetGainOptionsWidget =
      new OffsetGainOptionsWidget(&mOffsetGainConvert);
};

#endif // GRAY16_WINDOW_H
