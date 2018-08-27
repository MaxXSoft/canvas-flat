#ifndef CANVASFLAT_SHAPE_SHAPE_H_
#define CANVASFLAT_SHAPE_SHAPE_H_

#include <memory>
#include <vector>

#include "../color/color.h"

namespace cvf::shape {

struct Rect {
    Rect() : left(0), top(0), right(0), bottom(0) {}
    Rect(int left, int top, int right, int bottom)
            : left(left), top(top), right(right), bottom(bottom) {}
    int left, top, right, bottom;
};

class Shape {
public:
    virtual ~Shape() = default;

    virtual float GetSDF(float x, float y) const = 0;
    virtual Rect GetDrawArea() const = 0;

    void set_color(const Color &color) { color_ = color; }
    const Color &color() const { return color_; }

protected:
    Color color_;
};

using ShapePtr = std::shared_ptr<Shape>;
using ShapeList = std::vector<ShapePtr>;

} // namespace cvf::shape

#endif // CANVASFLAT_SHAPE_SHAPE_H_
