#ifndef CANVASFLAT_CONTAINER_PPMCONT_H_
#define CANVASFLAT_CONTAINER_PPMCONT_H_

#include "imgcontainer.h"

namespace cvf::container {

class PpmContainer : public ImageContainer {
public:
    enum class Format {
        PBM,   // Portable Bit Map,   monochrome
        PGM,   // Portable Gray Map,  grayscale
        PPM    // Portable Pixel Map, color
    };

    PpmContainer() : format_(Format::PPM), binary_(false) {}
    PpmContainer(Format format) : format_(format), binary_(false) {}
    PpmContainer(Format format, bool binary)
            : format_(format), binary_(binary) {}

protected:
    void ExportStream(std::ofstream &ofs) override {
        ofs.sync_with_stdio(false);
        WriteHeader(ofs);
        if (binary_) {
            WriteBodyBinary(ofs);
        }
        else {
            WriteBodyASCII(ofs);
        }
    }

private:
    unsigned char GetGrayscale(int pos) {
        const auto cr = 0.2126F, cg = 0.7152F, cb = 0.0722F;
        unsigned char gr = buffer_[pos] * cr;
        unsigned char gg = buffer_[pos + 1] * cg;
        unsigned char gb = buffer_[pos + 2] * cb;
        return gr + gg + gb;
    }

    unsigned char GetOtsuThreshold() {
        // get grayscale histogram
        int gray[256];
        for (auto &&i : gray) i = 0;
        for (int i = 0; i < width_ * height_ * 3; i += 3) {
            ++gray[GetGrayscale(i)];
        }
        // get sum of each greyscale pixel
        int pixel_sum = 0;
        for (int i = 0; i < 256; ++i) pixel_sum += i * gray[i];
        // try each threshold
        int sum = width_ * height_;
        float last_sum = 0, last_pixel_sum = 0, var_between_max = 0;
        unsigned char threshold = 0;
        for (int i = 1; i < 256; ++i) {
            last_sum += gray[i - 1];
            last_pixel_sum += (i - 1) * gray[i - 1];
            // calculate weight
            auto w1 = last_sum / sum, w2 = 1 - w1;
            // calculate average
            auto u1 = last_pixel_sum / last_sum;
            auto u2 = (pixel_sum - last_pixel_sum) / (sum - last_sum);
            // calculate between class variance
            auto var_between = w1 * w2 * (u1 - u2) * (u1 - u2);
            // find maximum
            if (var_between > var_between_max) {
                var_between_max = var_between;
                threshold = i;
            }
        }
        return threshold;
    }

    void WriteHeader(std::ofstream &ofs) {
        ofs.put('P');
        switch (format_) {
            case Format::PBM: {
                ofs.put('1' + (binary_ ? 3 : 0));
                ofs << '\n' << width_ << ' ' << height_ << '\n';
                break;
            }
            case Format::PGM: {
                ofs.put('2' + (binary_ ? 3 : 0));
                ofs << '\n' << width_ << ' ' << height_ << "\n255\n";
                break;
            }
            case Format::PPM: {
                ofs.put('3' + (binary_ ? 3 : 0));
                ofs << '\n' << width_ << ' ' << height_ << "\n255\n";
                break;
            }
        }
    }

    void WriteBodyASCII(std::ofstream &ofs) {
        switch (format_) {
            case Format::PBM: {
                auto threshold = GetOtsuThreshold();
                for (int y = 0; y < height_; ++y) {
                    for (int x = 0; x < width_; ++x) {
                        auto gray = GetGrayscale((y * width_ + x) * 3);
                        ofs << (gray < threshold) << ' ';
                    }
                    ofs.put('\n');
                }
                break;
            }
            case Format::PGM: {
                for (int y = 0; y < height_; ++y) {
                    for (int x = 0; x < width_; ++x) {
                        int gray = GetGrayscale((y * width_ + x) * 3);
                        ofs << gray << ' ';
                    }
                    ofs.put('\n');
                }
                break;
            }
            case Format::PPM: {
                for (int y = 0; y < height_; ++y) {
                    for (int x = 0; x < width_; ++x) {
                        auto p = buffer_ + (y * width_ + x) * 3;
                        ofs << static_cast<int>(p[0]) << ' ';
                        ofs << static_cast<int>(p[1]) << ' ';
                        ofs << static_cast<int>(p[2]) << ' ';
                    }
                    ofs.put('\n');
                }
                break;
            }
        }
    }

    void WriteBodyBinary(std::ofstream &ofs) {
        switch (format_) {
            case Format::PBM: {
                auto threshold = GetOtsuThreshold();
                char temp_byte = 0, cur_pos = 7;
                for (int y = 0; y < height_; ++y) {
                    for (int x = 0; x < width_; ++x) {
                        auto gray = GetGrayscale((y * width_ + x) * 3);
                        temp_byte |= (gray < threshold) << (cur_pos--);
                        if (cur_pos < 0) {
                            ofs.put(temp_byte);
                            temp_byte = 0;
                            cur_pos = 7;
                        }
                    }
                }
                if (cur_pos >= 0 && cur_pos < 7) ofs.put(temp_byte);
                break;
            }
            case Format::PGM: {
                for (int y = 0; y < height_; ++y) {
                    for (int x = 0; x < width_; ++x) {
                        ofs.put(GetGrayscale((y * width_ + x) * 3));
                    }
                }
                break;
            }
            case Format::PPM: {
                for (int y = 0; y < height_; ++y) {
                    for (int x = 0; x < width_; ++x) {
                        auto p = buffer_ + (y * width_ + x) * 3;
                        ofs.put(p[0]);
                        ofs.put(p[1]);
                        ofs.put(p[2]);
                    }
                }
                break;
            }
        }
    }

    Format format_;
    bool binary_;
};

} // namespace cvf::container

#endif // CANVASFLAT_CONTAINER_PPMCONT_H_
