#ifndef SHAPE_RECTANGLE_H_
#define SHAPE_RECTANGLE_H_

#include <cmath>

#include "shape.h"

namespace cvf::shape {

class Rectangle : public Shape {
public:
    Rectangle(float x0, float y0, float width, float height)
            : x0_(x0), y0_(y0), width_(width), height_(height) {}
    Rectangle(float x0, float y0, float side)
            : x0_(x0), y0_(y0), width_(side), height_(side) {}

    float GetSDF(float x, float y) const override {
        auto center_x = x0_ + width_ / 2, center_y = y0_ + height_ / 2;
        auto dx = std::fabsf(x - center_x) - width_;
        auto dy = std::fabsf(y - center_y) - height_;
        return std::fminf(std::fmaxf(dx, dy), 0.F)
                + std::sqrtf(std::powf(std::fmaxf(dx, 0.F), 2.F)
                + std::powf(std::fmaxf(dy, 0.F), 2.F));
        // bool in_rect = x >= x0_ && x <= x0_ + width_
        //         && y >= y0_ && y <= y0_ + height_;
        // auto d_left = std::fabsf(x - x0_);
        // auto d_right = std::fabsf(x - x0_ - width_);
        // auto d_top = std::fabsf(y - y0_);
        // auto d_bottom = std::fabsf(y - y0_ - height_);
        // auto d = std::fminf(std::fminf(
        //         std::fminf(d_left, d_right), d_top), d_bottom);
        // return in_rect ? -d : d;
    }

    Rect GetDrawArea() const override {
        auto x0 = std::floorf(x0_);
        auto y0 = std::floorf(y0_);
        auto x1 = std::ceilf(x0_ + width_);
        auto y1 = std::ceilf(y0_ + height_);
        return Rect(x0, y0, x1, y1);
    }

private:
    float x0_, y0_, width_, height_;
};

} // namespace cvf::shape

#endif // SHAPE_RECTANGLE_H_
