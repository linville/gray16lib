/*!
  \file image_histogram_widget.h
  \brief Definition of the ImageHistogramWidget class.
*/

#ifndef IMAGE_HISTOGRAM_WIDGET_H
#define IMAGE_HISTOGRAM_WIDGET_H

#include <QtCore>
#include <QtWidgets>

/*!
  \brief Widget that draws a histogram of a 16-bit grayscale QImage.
*/

class ImageHistogramWidget : public QWidget {
  Q_OBJECT

public:
  explicit ImageHistogramWidget(QWidget *parent);

  void setImage(const QImage &image);
  void highlightValue(int value);

  QSize minimumSizeHint() const override;
  QSize sizeHint() const override;

  static QVector<int> calculateHistogram(const QImage &image,
                                         int bins,
                                         int &outMaxHistogramValue);
  static QPixmap drawHistogram(const QVector<int> histogram,
                               int height,
                               int maxHistogramValue = -1);

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  QPixmap mGetCachedHistogram();

  QImage mImage; //!< QImage to generate histogram from.
  int mHighlightValue; //!< Bin value which to draw highlight cursor

  // Caching
  QVector<int> mHistogram; //!< Histogram (kept around for repaints).
  int mMaxHistogramValue = 0; //!< Value of largest bin in histogram.
  QPixmap mHistogramPixmap; //!< Pixmap of histogram

  // Drawing
  QPen mCursorPen; //!< Cursor Pen
};

#endif // IMAGE_HISTOGRAM_WIDGET_H

