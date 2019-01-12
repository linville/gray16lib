/*!
  \file neighbor_scale_options_widget.h
  \brief Definition/Implementation of the NeighborScaleOptionsWidget
*/

#ifndef NEIGHBOR_SCALE_OPTIONS_WIDGET_H
#define NEIGHBOR_SCALE_OPTIONS_WIDGET_H

#include "ui_neighbor_scale_options_widget_base.h"
#include <QtWidgets>

#include "gray16lib.h"

/*!
  \brief GUI options for the ManualConvert class.
*/

class NeighborScaleOptionsWidget : public QWidget {
  Q_OBJECT

public:
  explicit NeighborScaleOptionsWidget(Gray16::NeighborScale *neighborScaleConverter)
    : QWidget(), mNeighborScaleConverter(neighborScaleConverter) {
    ui.setupUi(this);

    ui.neighborDistanceSlider->setRange(1, 16);

    ui.neighborDistanceSlider->setValue(4);
    ui.neighborDistanceEdit->setText(QString::number(4));
    neighborScaleConverter->setCompareDistance(4);

    connect(ui.neighborDistanceSlider, QOverload<int>::of(&QSlider::valueChanged),
            this, [this](int value) {
      ui.neighborDistanceEdit->setText(QString::number(value));
      mNeighborScaleConverter->setCompareDistance(value);

      emit optionsUpdated();
    });
  }

signals:
  void optionsUpdated();

private:
  Ui::NeighborScaleOptionsWidgetBase ui;
  Gray16::NeighborScale *mNeighborScaleConverter;
};

#endif // NEIGHBOR_SCALE_OPTIONS_WIDGET_H
