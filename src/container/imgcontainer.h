#ifndef CONTAINER_IMGCONTAINER_H_
#define CONTAINER_IMGCONTAINER_H_

#include <fstream>
#include <memory>

namespace cvf::container {

class ImageContainer {
public:
    virtual ~ImageContainer() = default;

    void ReadBuffer(const unsigned char *buffer, int width, int height) {
        buffer_ = buffer;
        width_ = width;
        height_ = height;
    }

    void Export(const char *path) {
        std::ofstream ofs(path, std::ios::binary);
        if (ofs.is_open()) {
            ExportStream(ofs);
        }
    }

protected:
    ImageContainer() : buffer_(nullptr), width_(0), height_(0) {}

    virtual void ExportStream(std::ofstream &ofs) = 0;

    const unsigned char *buffer_;
    int width_, height_;
};

using ImageContainerPtr = std::unique_ptr<ImageContainer>;

} // namespace cvf::container

#endif // CONTAINER_IMGCONTAINER_H_
