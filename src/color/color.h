#ifndef CANVASFLAT_COLOR_COLOR_H_
#define CANVASFLAT_COLOR_COLOR_H_

#include <cstddef>
#include <functional>
#include <cmath>

#include "solid.h"
#include "../util/mathutil.h"

namespace cvf::color {

class Color {
public:
    using ColorFunction = std::function<SolidColor(float, float)>;

    enum class ColorType : char {
        Solid, Linear, Radial, Functional
    };

    Color() : color_type_(ColorType::Solid),
              color1_(nullptr), color2_(nullptr),
              start_(0.F), end_(0.F), radian_(0.F),
              color_func_(nullptr) {}
    Color(std::nullptr_t)
            : color_type_(ColorType::Solid),
              color1_(nullptr), color2_(nullptr),
              start_(0.F), end_(0.F), radian_(0.F),
              color_func_(nullptr) {}
    Color(SolidColor color)
            : color_type_(ColorType::Solid),
              color1_(color), color2_(nullptr),
              start_(0.F), end_(0.F), radian_(0.F),
              color_func_(nullptr) {}
    Color(Color24b rgb)
            : color_type_(ColorType::Solid),
              color1_(rgb), color2_(nullptr),
              start_(0.F), end_(0.F), radian_(0.F),
              color_func_(nullptr) {}
    Color(Color24b rgb, float alpha)
            : color_type_(ColorType::Solid),
              color1_(rgb, alpha), color2_(nullptr),
              start_(0.F), end_(0.F), radian_(0.F),
              color_func_(nullptr) {}
    Color(SolidColor color1, SolidColor color2)
            : color_type_(ColorType::Linear),
              color1_(color1), color2_(color2),
              start_(0.F), end_(1.F), radian_(util::PI_2),
              color_func_(nullptr) {}
    Color(Color24b rgb1, Color24b rgb2)
            : color_type_(ColorType::Linear),
              color1_(rgb1), color2_(rgb2),
              start_(0.F), end_(1.F), radian_(util::PI_2),
              color_func_(nullptr) {}
    Color(SolidColor color1, SolidColor color2, float radian)
            : color_type_(ColorType::Linear),
              color1_(color1), color2_(color2),
              start_(0.F), end_(1.F),
              radian_(util::RadiansNormalize(radian)),
              color_func_(nullptr) {}
    Color(ColorType color_type,
            SolidColor color1, SolidColor color2,
            float start, float end, float radian)
            : color_type_(color_type),
              color1_(color1), color2_(color2),
              start_(start), end_(end),
              radian_(util::RadiansNormalize(radian)),
              color_func_(nullptr) {}
    Color(ColorFunction color_func)
            : color_type_(ColorType::Functional),
              color1_(nullptr), color2_(nullptr),
              start_(0.F), end_(0.F), radian_(0.F),
              color_func_(color_func) {}

    SolidColor GetColor() const {
        return is_solid() ? color1_ : nullptr;
    }

    SolidColor GetColor(float percent_x, float percent_y) const {
        float percent;
        switch (color_type_) {
            case ColorType::Solid: {
                return color1_;
            }
            case ColorType::Functional: {
                return color_func_(percent_x, percent_y);
            }
            case ColorType::Linear: {
                if (util::FloatEqual(radian_, 0)) {
                    percent = percent_x;
                }
                else if (util::FloatEqual(radian_, util::PI_2)) {
                    percent = percent_y;
                }
                else if (util::FloatEqual(radian_, util::PI)) {
                    percent = 1 - percent_x;
                }
                else if (util::FloatEqual(radian_, 3 * util::PI_2)) {
                    percent = 1 - percent_y;
                }
                else {
                    auto dx = percent_x - 0.5F, dy = percent_y - 0.5F;
                    auto rho = std::sqrtf(dx * dx + dy * dy);
                    float theta = std::atan2f(dy, dx);
                    theta -= radian_;
                    percent = rho * std::cosf(theta) + 0.5F;
                }
                break;
            }
            case ColorType::Radial: {
                auto dx = percent_x - 0.5F, dy = percent_y - 0.5F;
                auto d = std::sqrtf(dx * dx + dy * dy);
                percent = util::LinearMapping(d, 0, std::sqrtf(0.5), 0, 1);
                break;
            }
        }
        // gradient color
        percent = util::LinearMapping(percent, start_, end_, 0, 1);
        auto r = color1_.red * (1 - percent) + color2_.red * percent;
        auto g = color1_.green * (1 - percent) + color2_.green * percent;
        auto b = color1_.blue * (1 - percent) + color2_.blue * percent;
        auto a = color1_.alpha * (1 - percent) + color2_.alpha * percent;
        return SolidColor(r, g, b, a);
    }

    Color &operator=(const SolidColor &solid) {
        color_type_ = ColorType::Solid;
        color1_ = solid;
        color2_ = nullptr;
        start_ = end_ = radian_ = 0.F;
        color_func_ = nullptr;
        return *this;
    }

    bool is_solid() const { return color_type_ == ColorType::Solid; }

private:
    ColorType color_type_;
    SolidColor color1_, color2_;
    float start_, end_, radian_;
    ColorFunction color_func_;
};

} // namespace cvf::color

#endif // CANVASFLAT_COLOR_COLOR_H_
