#ifndef CANVASFLAT_COLOR_SOLID_H_
#define CANVASFLAT_COLOR_SOLID_H_

#include <cstddef>
#include <cmath>
#include <limits>

namespace cvf::color {

// type definitions
using Color8b = unsigned char;
using Color24b = unsigned int;

// structure of solid color
struct SolidColor {
    SolidColor() : red(0), green(0), blue(0), alpha(0.F) {}
    SolidColor(std::nullptr_t) : red(0), green(0), blue(0), alpha(0.F) {}
    SolidColor(Color8b red, Color8b green, Color8b blue)
            : red(red), green(green), blue(blue), alpha(1.F) {}
    SolidColor(Color8b red, Color8b green, Color8b blue, float alpha)
            : red(red), green(green), blue(blue), alpha(alpha) {}
    SolidColor(Color24b rgb) { SetRGBA(rgb, 1.F); }
    SolidColor(Color24b rgb, float alpha) { SetRGBA(rgb, alpha); }

    Color24b GetRGB() const {
        return (red << 16) | (green << 8) | blue;
    }

    SolidColor &operator=(Color24b rgb) {
        SetRGBA(rgb, 1.F);
        return *this;
    }

    SolidColor &operator=(std::nullptr_t) {
        SetRGBA(0, 0.F);
        return *this;
    }

    operator bool() { return !IsTransparent(); }

    bool operator==(Color24b rgb) {
        return IsOpaque() && GetRGB() == rgb;
    }

    bool operator==(std::nullptr_t) { return IsTransparent(); }

    Color8b red, green, blue;
    float alpha;

private:
    void SetRGBA(Color24b rgb, float alpha) {
        red = (rgb >> 16) & 0xff;
        green = (rgb >> 8) & 0xff;
        blue = rgb & 0xff;
        this->alpha = std::fabsf(alpha);
        if (IsOpaque()) this->alpha = 1.F;
    }

    bool IsTransparent() {
        // assert alpha > 0
        return alpha <= (alpha < 0.F ? 0.F : alpha)
                * std::numeric_limits<float>::epsilon();
    }

    bool IsOpaque() {
        // assert alpha > 0
        return (alpha - 1.F) >= (alpha < 1.F ? 1.F : alpha)
                * std::numeric_limits<float>::epsilon();
    }
};

} // namespace cvf::color

#endif // CANVASFLAT_COLOR_SOLID_H_
