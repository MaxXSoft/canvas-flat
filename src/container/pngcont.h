#ifndef CONTAINER_PNGCONT_H_
#define CONTAINER_PNGCONT_H_

#include "imgcontainer.h"

#define SVPNG_OUTPUT std::ofstream &ofs
#define SVPNG_PUT(u) ofs.put(u)
#include "png/svpng.inc"

namespace cvf::container {

class PngContainer : public ImageContainer {
public:
    PngContainer() : ImageContainer() {}

protected:
    void ExportStream(std::ofstream &ofs) override {
        if (buffer_ && width_ && height_) {
            svpng(ofs, width_, height_, buffer_, false);
        }
    }
};

} // namespace cvf::container

#endif // CONTAINER_PNGCONT_H_
