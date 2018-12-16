/*!
  \file false_color_options_widget.h
  \brief Definition/Implementation of the FalseColorOptionsWidget
*/

#ifndef FALSE_COLOR_OPTIONS_WIDGET_H
#define FALSE_COLOR_OPTIONS_WIDGET_H

#include "ui_false_color_options_widget_base.h"
#include <QtWidgets>

#include "gray16lib.h"

/*!
  \brief GUI options for the FalseColorConvert class.
*/

class FalseColorOptionsWidget : public QWidget {
  Q_OBJECT

public:
  explicit FalseColorOptionsWidget(Gray16::FalseColorConvert *falseColorConverter)
    : QWidget(), mFalseColorConverter(falseColorConverter) {
    ui.setupUi(this);

    ui.gradientSchemeCombo->addItem("Black Body");
  }

signals:
  void optionsUpdated();

private:
  void mOnGradientSchemeUpdated(const QString &name) {
    using FalseColorMode = Gray16::FalseColorConvert::FalseColorMode;

    if(name == "Black Body") {
      mFalseColorConverter->setFalseColorMode(FalseColorMode::BlackBody);
    } else {
      qCritical() << "Unknown name:" << name;
      return;
    }

    emit optionsUpdated();
  }

  Ui::FalseColorOptionsWidgetBase ui;
  Gray16::FalseColorConvert *mFalseColorConverter;
};

#endif // FALSE_COLOR_OPTIONS_WIDGET_H
