#ifndef CANVASFLAT_RENDER_BASIC_H_
#define CANVASFLAT_RENDER_BASIC_H_

#include <iostream>

#include "render.h"
#include "../util/mathutil.h"

namespace cvf::render {

class BasicRender : public Render {
public:
    BasicRender() : Render() {}

    void Redraw(const color::Color &backcolor,
            const shape::ShapeList &shapes) override {
        // TODO: show progress
        // draw the background
        DrawBackground(backcolor);
        // draw shapes
        for (const auto &shape : shapes) {
            DrawShape(shape);
        }
    }

private:
    void DrawBackground(const color::Color &backcolor) {
        auto p = buffer_;
        if (backcolor.is_solid()) {
            auto rgba = backcolor.GetColor();
            for (int y = 0; y < height_; ++y) {
                for (int x = 0; x < width_; ++x) {
                    *p++ = rgba.red;
                    *p++ = rgba.green;
                    *p++ = rgba.blue;
                }
            }
        }
        else {
            for (int y = 0; y < height_; ++y) {
                for (int x = 0; x < width_; ++x) {
                    auto px = static_cast<float>(x) / width_;
                    auto py = static_cast<float>(y) / height_;
                    // get non-solid color
                    auto rgba = backcolor.GetColor(px, py);
                    *p++ = rgba.red;
                    *p++ = rgba.green;
                    *p++ = rgba.blue;
                }
            }
        }
    }

    void DrawShape(const shape::ShapePtr &shape) {
        // get draw area
        shape::Rect area = shape->GetDrawArea(), draw;
        draw.left = util::Max(area.left, 0);
        draw.top = util::Max(area.top, 0);
        draw.right = util::Min(area.right, width_ - 1);
        draw.bottom = util::Min(area.bottom, height_ - 1);
        // draw pixels in area
        auto color = shape->color();
        if (color.is_solid()) {
            auto rgba = color.GetColor();
            for (int y = draw.top; y <= draw.bottom; ++y) {
                for (int x = draw.left; x <= draw.right; ++x) {
                    // calculate the pointer of current pixel
                    auto p = buffer_ + (y * width_ + x) * 3;
                    // draw pixel
                    auto alpha = GetPixelVisible(x, y, shape) * rgba.alpha;
                    if (alpha > 0.F) {
                        AlphaBlendX(p[0], rgba.red, alpha);
                        AlphaBlendX(p[1], rgba.green, alpha);
                        AlphaBlendX(p[2], rgba.blue, alpha);
                    }
                }
            }
        }
        else {   // non-solid color
            float aw = area.right - area.left + 1;
            float ah = area.bottom - area.top + 1;
            for (int y = draw.top; y <= draw.bottom; ++y) {
                for (int x = draw.left; x <= draw.right; ++x) {
                    // calculate the pointer of current pixel
                    auto p = buffer_ + (y * width_ + x) * 3;
                    // get current color
                    auto rgba = color.GetColor((x - area.left) / aw,
                            (y - area.top) / ah);
                    // draw pixel
                    auto alpha = GetPixelVisible(x, y, shape) * rgba.alpha;
                    if (alpha > 0.F) {
                        AlphaBlendX(p[0], rgba.red, alpha);
                        AlphaBlendX(p[1], rgba.green, alpha);
                        AlphaBlendX(p[2], rgba.blue, alpha);
                    }
                }
            }
        }
    }
};

} // namespace cvf::render

#endif // CANVASFLAT_RENDER_BASIC_H_
