#ifndef CANVASFLAT_SHAPE_RECTANGLE_H_
#define CANVASFLAT_SHAPE_RECTANGLE_H_

#include <cmath>

#include "shape.h"

namespace cvf::shape {

class Rectangle : public Shape {
public:
    Rectangle(float x0, float y0, float width, float height)
            : x0_(x0), y0_(y0), width_(width), height_(height) {
        InitParam();
    }
    Rectangle(float x0, float y0, float side)
            : x0_(x0), y0_(y0), width_(side), height_(side) {
        InitParam();
    }

    float GetSDF(float x, float y) const override {
        auto dx = std::fabsf(x - cx_) - sx_, ax = std::fmaxf(dx, 0.F);
        auto dy = std::fabsf(y - cy_) - sy_, ay = std::fmaxf(dy, 0.F);
        return std::fminf(std::fmaxf(dx, dy), 0.F)
                + std::sqrtf(ax * ax + ay * ay);
    }

    Rect GetDrawArea() const override {
        auto x0 = std::floorf(x0_);
        auto y0 = std::floorf(y0_);
        auto x1 = std::ceilf(x0_ + width_);
        auto y1 = std::ceilf(y0_ + height_);
        return Rect(x0, y0, x1, y1);
    }

private:
    void InitParam() {
        sx_ = width_ / 2;
        sy_ = width_ / 2;
        cx_ = x0_ + sx_;
        cy_ = y0_ + sy_;
    }

    float x0_, y0_, width_, height_;
    float cx_, cy_, sx_, sy_;
};

} // namespace cvf::shape

#endif // CANVASFLAT_SHAPE_RECTANGLE_H_
