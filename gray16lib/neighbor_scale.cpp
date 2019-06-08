/*!
  \file local_compare.cpp
  \brief Implementation of the LocalCompare class.
*/

#include "neighbor_scale.h"

#include <QtDebug>
#include <QImage>

#include <algorithm>
#include <cmath>
#include <tuple>

using namespace Gray16;

void NeighborScale::setCompareDistance(int distance) {
  Q_ASSERT(distance > 0);

  mDistance = distance;
}

QImage NeighborScale::mConvert(const QImage &originalImage) const {
  QImage image(originalImage.width(), originalImage.height(), QImage::Format_Grayscale8);

  uint8_t *d = reinterpret_cast<uint8_t *> (image.bits());
  const uint16_t *src = reinterpret_cast<const uint16_t *> (originalImage.constBits());

  for(int y = 0; y < originalImage.height(); ++y) {
    for(int x = 0; x < originalImage.width(); ++x) {
      const auto [blackPoint, whitePoint] = mFindLocalMinMax(originalImage, x, y, mDistance);

      const auto scale = (whitePoint - blackPoint) / 255.;
      const auto offset = (y * image.width()) + x;

      const int value = std::lrint((src[offset] - blackPoint) / scale);
      d[offset] = std::clamp(value, 0, 255);
    }
  }

  return image;
}

std::tuple<uint16_t, uint16_t> NeighborScale::mFindLocalMinMax(const QImage &image, int refX, int refY, int distance) {
  uint16_t outMin = 0xffff;
  uint16_t outMax = 0x0000;

  const uint16_t *src = reinterpret_cast<const uint16_t *> (image.constBits());

  for(int y = refY - distance; y < refY + distance; ++y) {
    if(y < 0) {
      continue;
    } else if(y >= image.height()) {
      break;
    }

    for(int x = refX - distance; x < refX + distance; ++x) {
      if(x < 0) {
        continue;
      } else if(x >= image.width()) {
        break;
      }

      // Don't include reference pixel
//      if(x == refX &&
//         y == refY) {
//        continue;
//      }

      const auto offset = (y * image.width()) + x;
      outMin = std::min(outMin,
                        src[offset]);
      outMax = std::max(outMax,
                        src[offset]);
    }
  }

  Q_ASSERT(outMin <= outMax);
  return std::make_tuple(outMin, outMax);
}
