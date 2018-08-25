#ifndef SHAPE_CAPSULE_H_
#define SHAPE_CAPSULE_H_

#include <cmath>

#include "shape.h"

class Capsule : public Shape {
public:
    Capsule(float x0, float y0, float x1, float y1, float r)
            : x0_(x0), y0_(y0), x1_(x1), y1_(y1), r_(r) {}

    float GetSDF(float x, float y) const override {
        auto dx0 = x - x0_, dy0 = y - y0_, dx1 = x1_ - x0_, dy1 = y1_ - y0_;
        auto h = (dx0 * dx1 + dy0 * dy1) / (dx1 * dx1 + dy1 * dy1);
        h = std::fmaxf(std::fminf(h, 1.F), 0.F);
        auto dx = dx0 - dx1 * h, dy = dy0 - dy1 * h;
        return std::sqrtf(dx * dx + dy * dy) - r_;
    }

    Rect GetDrawArea() const override {
        auto x0 = std::floorf(std::fminf(x0_, x1_) - r_);
        auto y0 = std::floorf(std::fminf(y0_, y1_) - r_);
        auto x1 = std::ceilf(std::fmaxf(x0_, x1_) + r_);
        auto y1 = std::ceilf(std::fmaxf(y0_, y1_) + r_);
        return Rect(x0, y0, x1, y1);
    }

private:
    float x0_, y0_, x1_, y1_, r_;
};

#endif // SHAPE_CAPSULE_H_
