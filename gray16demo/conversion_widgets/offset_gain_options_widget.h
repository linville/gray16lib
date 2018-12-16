/*!
  \file offset_gain_options_widget.h
  \brief Definition/Implementation of the OffsetGainOptionsWidget
*/

#ifndef OFFSET_GAIN_OPTIONS_WIDGET_H
#define OFFSET_GAIN_OPTIONS_WIDGET_H

#include "ui_offset_gain_options_widget_base.h"
#include <QtWidgets>

#include "gray16lib.h"

/*!
  \brief GUI options for the OffsetGainConvert class.
*/

class OffsetGainOptionsWidget : public QWidget {
  Q_OBJECT

public:
  explicit OffsetGainOptionsWidget(Gray16::OffsetGainConvert *offsetGainConverter)
    : QWidget(), mOffsetGainConverter(offsetGainConverter) {
    ui.setupUi(this);

    QStringList modes;
    modes << "Manual"
          << "Offset by Least-Exposed"
          << "Offset by Most-Exposed";
    ui.offsetModeCombo->addItems(modes);

    ui.offsetSlider->setRange(-65280, 65280);
    ui.gainSlider->setRange(-4, 4);

    connect(ui.offsetModeCombo, &QComboBox::currentTextChanged,
            this, &OffsetGainOptionsWidget::mOnUpdate);
    connect(ui.offsetSlider, QOverload<int>::of(&QSlider::valueChanged),
            this, &OffsetGainOptionsWidget::mOnUpdate);
    connect(ui.gainSlider, QOverload<int>::of(&QSlider::valueChanged),
            this, &OffsetGainOptionsWidget::mOnUpdate);

    mUpdateEdits();
  }

  void updateOffset(int offset) {
    ui.offsetSlider->blockSignals(true);
    ui.offsetSlider->setValue(offset);
    ui.offsetEdit->setText(QString::number(offset));
    ui.offsetSlider->blockSignals(false);

    mOffsetGainConverter->setOffsetGain(ui.offsetSlider->value(),
                                        ui.gainSlider->value());
  }

signals:
  void optionsUpdated();

private:
  void mOnUpdate() {
    const auto textMode = ui.offsetModeCombo->currentText();

    using OffsetMode = Gray16::OffsetGainConvert::OffsetMode;

    if(textMode == "Manual") {
      ui.offsetSlider->setEnabled(true);
      mOffsetGainConverter->setOffsetMode(OffsetMode::Manual);
    } else if(textMode == "Offset by Least-Exposed") {
      ui.offsetSlider->setEnabled(false);
      mOffsetGainConverter->setOffsetMode(OffsetMode::LeastExposedOffset);
    } else if(textMode == "Offset by Most-Exposed") {
      ui.offsetSlider->setEnabled(false);
      mOffsetGainConverter->setOffsetMode(OffsetMode::MostExposedOffset);
    } else {
      qCritical() << "Unknown mode:" << textMode;
      return;
    }

    mUpdateEdits();

    mOffsetGainConverter->setOffsetGain(ui.offsetSlider->value(),
                                        ui.gainSlider->value());

    emit optionsUpdated();
  }

  void mUpdateEdits() {
    ui.offsetEdit->setText(QString::number(ui.offsetSlider->value()));
    ui.gainEdit->setText(QString::number(ui.gainSlider->value()));
  }

  Ui::OffsetGainOptionsWidgetBase ui;
  Gray16::OffsetGainConvert *mOffsetGainConverter;
};

#endif // OFFSET_GAIN_OPTIONS_WIDGET_H
