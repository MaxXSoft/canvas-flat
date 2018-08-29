#ifndef CANVASFLAT_CANVAS_H_
#define CANVASFLAT_CANVAS_H_

#include <vector>
#include <utility>

#include "color/color.h"
#include "container/imgcontainer.h"
#include "render/render.h"
#include "shape/shape.h"
#include "util/mathutil.h"

namespace cvf {

class Canvas {
public:
    Canvas(int width, int height) {
        set_size(width, height);
    }

    void Redraw() {
        render_->ReadBuffer(image_buffer_.data(), width_, height_);
        render_->Redraw(backcolor_, shapes_);
    }

    void Export(const char *path) {
        // reset the buffer info to prevent width & height changes
        image_container_->ReadBuffer(pixel(), width_, height_);
        image_container_->Export(path);
    }

    int AddShape(const shape::ShapePtr &shape) {
        shapes_.push_back(shape);
        return shapes_.size() - 1;
    }
    void ClearShape() { shapes_.clear(); }

    void set_size(int width, int height) {
        width_ = width;
        height_ = height;
        image_buffer_.resize(width_ * height_ * 3);
    }
    void set_backcolor(const color::Color &backcolor) {
        backcolor_ = backcolor;
    }
    void set_image_container(container::ImageContainerPtr image_container) {
        image_container_ = std::move(image_container);
    }
    void set_render(render::RenderPtr render) {
        render_ = std::move(render);
    }

    int width() const { return width_; }
    int height() const { return height_; }
    const color::Color &backcolor() const { return backcolor_; }
    const color::Color8b *pixel() const { return image_buffer_.data(); }
    const shape::ShapeList &shapes() const { return shapes_; }

private:
    using ImageBuffer = std::vector<color::Color8b>;

    int width_, height_;
    color::Color backcolor_;
    shape::ShapeList shapes_;
    ImageBuffer image_buffer_;
    container::ImageContainerPtr image_container_;
    render::RenderPtr render_;
};

} // namespace cvf

#endif // CANVASFLAT_CANVAS_H_
