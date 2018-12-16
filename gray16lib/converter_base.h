/*!
  \file converter_base.h
  \brief Definition of the ConverterBase class.
*/

#ifndef CONVERTER_BASE_H
#define CONVERTER_BASE_H

#include <QImage>

namespace Gray16 {
  /*!
    \brief ConverterBase - Base class for 16-bit Grayscale converters.
  */

  class ConverterBase {
  public:
    ConverterBase() = default;
    virtual ~ConverterBase() = default;

    virtual QImage convert(const QImage &originalImage) const;

    static int leastExposedValue(const QImage &image);
    static int mostExposedValue(const QImage &image);

  private:
    virtual QImage mConvert(const QImage &sourceImage) const = 0;
  };
}

#endif // CONVERTER_BASE_H
