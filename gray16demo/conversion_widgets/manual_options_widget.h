/*!
  \file manual_options_widget.h
  \brief Definition/Implementation of the ManualOptionsWidget
*/

#ifndef MANUAL_OPTIONS_WIDGET_H
#define MANUAL_OPTIONS_WIDGET_H

#include "ui_manual_options_widget_base.h"
#include <QtWidgets>

#include "gray16lib.h"

/*!
  \brief GUI options for the ManualConvert class.
*/

class ManualOptionsWidget : public QWidget {
  Q_OBJECT

public:
  explicit ManualOptionsWidget(Gray16::ManualConvert *manualConverter)
    : QWidget(), mManualConverter(manualConverter) {
    ui.setupUi(this);

    // Black Point always <= White Point
    ui.blackPointSlider->setRange(0,
                                  Gray16::ManualConvert::MaxBlackPoint);
    ui.whitePointSlider->setRange(Gray16::ManualConvert::MinWhitePoint,
                                  65535);

    ui.blackPointSlider->setValue(0);
    ui.whitePointSlider->setValue(65535);

    connect(ui.blackPointSlider, &QSlider::valueChanged,
            this, &ManualOptionsWidget::mOnUpdateBlackPoint);
    connect(ui.whitePointSlider, &QSlider::valueChanged,
            this, &ManualOptionsWidget::mOnUpdateWhitePoint);

    mUpdateEdits();
  }

signals:
  void optionsUpdated();

private:
  void mOnUpdateBlackPoint(int value) {
    if(value + 255 >= ui.whitePointSlider->value()) {
      ui.whitePointSlider->setValue(value + 255);
    }

    mUpdateEdits();

    mManualConverter->setExposurePoints(value,
                                        ui.whitePointSlider->value());

    emit optionsUpdated();
  }

  void mOnUpdateWhitePoint(int value) {
    if(value - 255 <= ui.blackPointSlider->value()) {
      ui.blackPointSlider->setValue(value - 255);
    }

    mUpdateEdits();

    mManualConverter->setExposurePoints(ui.blackPointSlider->value(),
                                        value);

    emit optionsUpdated();
  }

  void mUpdateEdits() {
    ui.blackPointEdit->setText(QString::number(ui.blackPointSlider->value()));
    ui.whitePointEdit->setText(QString::number(ui.whitePointSlider->value()));
  }

  Ui::ManualOptionsWidgetBase ui;
  Gray16::ManualConvert *mManualConverter;
};

#endif // MANUAL_OPTIONS_WIDGET_H
