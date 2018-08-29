#ifndef CANVASFLAT_RENDER_BASIC_H_
#define CANVASFLAT_RENDER_BASIC_H_

#include <cstdio>
#include <string>

#include "render.h"
#include "../util/mathutil.h"

namespace cvf::render {

class BasicRender : public Render {
public:
    BasicRender() : Render() {}

    void Redraw(const color::Color &backcolor,
            const shape::ShapeList &shapes) override {
        if (show_progress_) {
            shape_count_ = shapes.size();
            current_title_.reserve(64);
            // initialize progress bar
            progress_.set_count(2);
            progress_.set_title(0, "current: ready");
            progress_.set_title(1, "total:");
            progress_.Show();
            // async refresh progress bar
            auto task_refresh = progress_.RefreshAsync();
            auto task_render = std::thread(&BasicRender::RenderProcess,
                    this, backcolor, shapes);
            task_refresh.join();
            task_render.join();
        }
        else {
            RenderProcess(backcolor, shapes);
        }
    }

private:
    void RenderProcess(const color::Color &backcolor,
            const shape::ShapeList &shapes) {
        // draw the background
        DrawBackground(backcolor);
        // draw shapes
        int index = 0;
        for (const auto &shape : shapes) {
            DrawShape(index++, shape);
        }
        // complete
        if (show_progress_) {
            UpdateProgress(shape_count_, 0, 0, 1, 1);
        }
    }

    void UpdateProgress(int shape_index, int x, int y, int w, int h) {
        if (!show_progress_) return;
        auto percent = static_cast<float>(y * w + x) / (w * h);
        if (shape_index < 0) {
            current_title_ = "current: drawing background...";
            progress_.set_percent(0, percent);
            progress_.set_percent(1, percent / (1 + shape_count_));
        }
        else if (shape_index < shape_count_) {
            current_title_ = "current: drawing shapes... ";
            std::sprintf(shape_indicator_,
                    "%d/%d", shape_index + 1, shape_count_);
            current_title_ += shape_indicator_;
            progress_.set_percent(0, percent);
            progress_.set_percent(1,
                    (1 + shape_index + percent) / (1 + shape_count_));
        }
        else {
            current_title_ = "current: done";
            progress_.set_percent(0, 1);
            progress_.set_percent(1, 1);
        }
        progress_.set_title(0, current_title_);
    }

    void DrawBackground(const color::Color &backcolor) {
        auto p = buffer_;
        if (backcolor.is_solid()) {
            auto rgba = backcolor.GetColor();
            for (int y = 0; y < height_; ++y) {
                for (int x = 0; x < width_; ++x) {
                    UpdateProgress(-1, x, y, width_, height_);
                    *p++ = rgba.red;
                    *p++ = rgba.green;
                    *p++ = rgba.blue;
                }
            }
        }
        else {
            for (int y = 0; y < height_; ++y) {
                for (int x = 0; x < width_; ++x) {
                    UpdateProgress(-1, x, y, width_, height_);
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

    void DrawShape(int index, const shape::ShapePtr &shape) {
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
                    UpdateProgress(index, x, y,
                            draw.right - draw.left + 1,
                            draw.bottom - draw.top + 1);
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
                    UpdateProgress(index, x, y,
                            draw.right - draw.left + 1,
                            draw.bottom - draw.top + 1);
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

    int shape_count_;
    char shape_indicator_[32];
    std::string current_title_;
};

} // namespace cvf::render

#endif // CANVASFLAT_RENDER_BASIC_H_
