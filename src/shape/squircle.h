#ifndef CANVASFLAT_SHAPE_SQUIRCLE_H_
#define CANVASFLAT_SHAPE_SQUIRCLE_H_

#include <cmath>

#include "shape.h"

namespace cvf::shape {

class Squircle : public Shape {
public:
    Squircle(float center_x, float center_y, float r)
            : center_x_(center_x), center_y_(center_y),
              r_(r), order_(4.F) {}
    Squircle(float center_x, float center_y, float r, int order)
            : center_x_(center_x), center_y_(center_y),
              r_(r), order_(order * 2.F) {}

    float GetSDF(float x, float y) const override {
        auto dx = x - center_x_, dy = y - center_y_;
        auto pow_v = std::powf(dx, order_) + std::powf(dy, order_);
        return std::powf(pow_v, 1.F / order_) - r_;
    }

    Rect GetDrawArea() const override {
        auto x0 = std::floorf(center_x_ - r_);
        auto y0 = std::floorf(center_y_ - r_);
        auto x1 = std::ceilf(center_x_ + r_);
        auto y1 = std::ceilf(center_y_ + r_);
        return Rect(x0, y0, x1, y1);
    }

private:
    float center_x_, center_y_, r_, order_;
};

} // namespace cvf::shape

#endif // CANVASFLAT_SHAPE_SQUIRCLE_H_
