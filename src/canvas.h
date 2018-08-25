#ifndef CANVAS_H_
#define CANVAS_H_

#include <fstream>
#include <vector>
#include <cmath>

#include "color.h"
#include "shape/shape.h"

template <class ImgContainer>
class Canvas {
public:
    Canvas(int width, int height) : backcolor_(0U) {
        set_size(width, height);
        anti_aliasing_ = false;
    }

    void Redraw() {
        // draw background
        auto p = image_buffer_.data();
        for (int y = 0; y < height_; ++y) {
            for (int x = 0; x < width_; ++x) {
                *p++ = backcolor_.red;
                *p++ = backcolor_.green;
                *p++ = backcolor_.blue;
            }
        }
        // draw shapes
        for (const auto &i : shapes_) {
            auto area = i->GetDrawArea();
            // for (int y = 0; y < 512; ++y) {
            //     for (int x = 0; x < 512; ++x) {
            for (int y = area.top; y <= area.bottom; ++y) {
                for (int x = area.left; x <= area.right; ++x) {
                    if (x < 0 || y < 0 || x >= width_ || y >= height_) {
                        continue;
                    }
                    auto p = image_buffer_.data() + (y * width_ + x) * 3;
                    auto pixel = GetPixel(x, y, i);
                    if (pixel) {
                        AlphaBlendX(p[0], pixel.red, pixel.alpha);
                        AlphaBlendX(p[1], pixel.green, pixel.alpha);
                        AlphaBlendX(p[2], pixel.blue, pixel.alpha);
                    }
                }
            }
        }
    }

    void Export(const char *path = "export.png") {
        ImgContainer img;
        img.ReadBuffer(pixel(), width_, height_);
        img.Export(path);
    }

    void AddShape(const ShapePtr &shape) { shapes_.push_back(shape); }
    void ClearShape() { shapes_.clear(); }

    void set_size(int width, int height) {
        width_ = width;
        height_ = height;
        image_buffer_.resize(width_ * height_ * 3);
    }
    void set_anti_aliasing(bool anti_aliasing) {
        anti_aliasing_ = anti_aliasing;
    }
    void set_backcolor(const Color &backcolor) { backcolor_ = backcolor; }

    int width() const { return width_; }
    int height() const { return height_; }
    bool anti_aliasing() const { return anti_aliasing_; }
    const Color &backcolor() const { return backcolor_; }
    const Color::Color8b *pixel() const { return image_buffer_.data(); }
    const std::vector<ShapePtr> &shapes() const { return shapes_; }

private:
    using ImageBuffer = std::vector<Color::Color8b>;

    void AlphaBlendX(Color::Color8b &x, Color::Color8b y, float alpha) {
        x = static_cast<Color::Color8b>(x * (1 - alpha) + y * alpha);
    }

    Color GetPixel(float x, float y, const ShapePtr &shape) {
        auto sdf = shape->GetSDF(x, y);
        auto color = shape->color();
        if (anti_aliasing_) {
            auto alpha = std::fmaxf(std::fminf(0.5F - sdf, 1.F), 0.F);
            return Color(color.GetRGB(), color.alpha * alpha);
        }
        else {
            return sdf <= 0.F ? color : nullptr;
        }
    }

    int width_, height_;
    bool anti_aliasing_;
    Color backcolor_;
    ImageBuffer image_buffer_;
    std::vector<ShapePtr> shapes_;
};

#endif // CANVAS_H_