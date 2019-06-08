/*!
  \file neighbor_scale.h
  \brief Definition of the NeighborScale class.
*/

#ifndef NEIGHBOR_SCALE_H
#define NEIGHBOR_SCALE_H

#include "converter_base.h"

#include <QImage>

#include <tuple>

namespace Gray16 {
  /*!
    \brief NeighborScale - Pixel is scaled based on values of neighbor pixels.

    The minimum and maximum value values for pixels are searched distance-pixels
    from the current pixel to create a black-point and a white-point.
  */

  class NeighborScale : public ConverterBase {
  public:
    NeighborScale() = default;
    void setCompareDistance(int distance);

  private:
    QImage mConvert(const QImage &originalImage) const override;

    static std::tuple<uint16_t, uint16_t> mFindLocalMinMax(const QImage &image, int refX, int refY, int distance);

    int mDistance = 0; //!< Radius, in pixels, to compare current pixel to.
  };
}

#endif // LOCAL_COMPARE_H
