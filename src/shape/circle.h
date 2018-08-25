#ifndef SHAPE_CIRCLE_H_
#define SHAPE_CIRCLE_H_

#include <cmath>

#include "shape.h"

class Circle : public Shape {
public:
    Circle(float center_x, float center_y, float r)
            : center_x_(center_x), center_y_(center_y), r_(r) {}

    float GetSDF(float x, float y) const override {
        auto dx = x - center_x_, dy = y - center_y_;
        return std::sqrtf(dx * dx + dy * dy) - r_;
    }

    Rect GetDrawArea() const override {
        auto x0 = std::floorf(center_x_ - r_);
        auto y0 = std::floorf(center_y_ - r_);
        auto x1 = std::ceilf(center_x_ + r_);
        auto y1 = std::ceilf(center_y_ + r_);
        return Rect(x0, y0, x1, y1);
    }

private:
    float center_x_, center_y_, r_;
};

#endif // SHAPE_CIRCLE_H_
