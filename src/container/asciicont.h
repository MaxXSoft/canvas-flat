#ifndef CANVASFLAT_CONTAINER_ASCIICONT_H_
#define CANVASFLAT_CONTAINER_ASCIICONT_H_

#include "imgcontainer.h"
#include "../util/mathutil.h"

namespace cvf::container {

class AsciiContainer : public ImageContainer {
public:
    AsciiContainer() : ImageContainer() {}

protected:
    void ExportStream(std::ofstream &ofs) override {
        for (int y = 0; y + 1 < height_; y += 2) {
            for (int x = 0; x < width_; ++x) {
                ofs.put(GetASCII(x, y));
            }
            ofs.put('\n');
        }
    }

private:
    const int grayscale_count = 70;
    const char *ascii_grayscale = "$@B%8&WM#*oahkbdpqwmZO0QLC"
            "JUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";

    char GetASCII(int x, int y) {
        const auto cr = 0.2126F, cg = 0.7152F, cb = 0.0722F;
        int pos[] = {
            (y * width_ + x) * 3,
            ((y + 1) * width_ + x) * 3,
            // ((y + 2) * width_ + x) * 3,
        };
        int gray = 0;
        for (const auto &i : pos) {
            gray += buffer_[i] * cr;
            gray += buffer_[i + 1] * cg;
            gray += buffer_[i + 2] * cb;
        }
        int i = util::LinearMapping(gray, 0, 255 * 2, 0, grayscale_count);
        return ascii_grayscale[i];
    }
};

} // namespace cvf::container

#endif // CANVASFLAT_CONTAINER_ASCIICONT_H_
