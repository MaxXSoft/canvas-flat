#ifndef CANVASFLAT_RENDER_RENDER_H_
#define CANVASFLAT_RENDER_RENDER_H_

#include <memory>

#include "../color/color.h"
#include "../shape/shape.h"
#include "../util/mathutil.h"

namespace cvf::render {

class Render {
public:
    virtual ~Render() = default;

    void ReadBuffer(unsigned char *buffer, int width, int height) {
        buffer_ = buffer;
        width_ = width;
        height_ = height;
    }

    virtual void Redraw(const color::Color &backcolor,
            const shape::ShapeList &shapes) = 0;

    void set_show_progress(bool show_progress) {
        show_progress_ = show_progress;
    }
    void set_anti_aliasing(bool anti_aliasing) {
        anti_aliasing_ = anti_aliasing;
    }

    bool anti_aliasing() const { return anti_aliasing_; }
    bool show_progress() const { return show_progress_; }

protected:
    Render() : buffer_(nullptr), width_(0), height_(0),
               anti_aliasing_(false), show_progress_(false) {}

    void AlphaBlendX(color::Color8b &x, color::Color8b y, float alpha) {
        x = static_cast<color::Color8b>(x * (1 - alpha) + y * alpha);
    }

    float GetPixelVisible(float x, float y, const shape::ShapePtr &shape) {
        auto sdf = shape->GetSDF(x, y);
        if (anti_aliasing_) {
            return util::LinearMapping(sdf, -0.5, 0.5, 1, 0);
        }
        else {
            return sdf <= 0.F ? 1.F : 0.F;
        }
    }

    unsigned char *buffer_;
    int width_, height_;
    bool anti_aliasing_, show_progress_;
};

using RenderPtr = std::unique_ptr<Render>;

} // namespace cvf::render

#endif // CANVASFLAT_RENDER_RENDER_H_
