#ifndef CANVASFLAT_SHAPE_OPERATION_H_
#define CANVASFLAT_SHAPE_OPERATION_H_

#include <cmath>

#include "shape.h"
#include "../util/mathutil.h"

namespace cvf::shape {

class Operation : public Shape {
public:
    enum class Opcode : char {
        Union, Intersection, Difference,
        Rotate, Scale, Round
    };

    Operation(Opcode opcode, ShapePtr opr1, ShapePtr opr2)
            : opcode_(opcode), opr1_(opr1), opr2_(opr2),
              center_x_(0.F), center_y_(0.F), param_(0.F) {}
    Operation(Opcode opcode, ShapePtr opr, float param)
            : opcode_(opcode), opr1_(opr), opr2_(nullptr), param_(param) {
        auto area = opr1_->GetDrawArea();
        center_x_ = area.left + (area.right - area.left) / 2;
        center_y_ = area.top + (area.bottom - area.top) / 2;
    }

    float GetSDF(float x, float y) const override {
        switch (opcode_) {
            case Opcode::Union: {
                return std::fminf(opr1_->GetSDF(x, y), opr2_->GetSDF(x, y));
            }
            case Opcode::Intersection: {
                return std::fmaxf(opr1_->GetSDF(x, y), opr2_->GetSDF(x, y));
            }
            case Opcode::Difference: {
                return std::fmaxf(opr1_->GetSDF(x, y), -opr2_->GetSDF(x, y));
            }
            case Opcode::Rotate: case Opcode::Scale: {
                CoordMapping(x, y);
                return opr1_->GetSDF(x, y);
            }
            case Opcode::Round: {
                return opr1_->GetSDF(x, y) - param_;
            }
        }
    }

    Rect GetDrawArea() const override {
        int x0, y0, x1, y1;
        switch (opcode_) {
            case Opcode::Union: {
                auto r1 = opr1_->GetDrawArea(), r2 = opr2_->GetDrawArea();
                x0 = util::Min(r1.left, r2.left);
                y0 = util::Min(r1.top, r2.top);
                x1 = util::Max(r1.right, r2.right);
                y1 = util::Max(r1.bottom, r2.bottom);
                break;
            }
            case Opcode::Intersection: {
                auto r1 = opr1_->GetDrawArea(), r2 = opr2_->GetDrawArea();
                x0 = util::Max(r1.left, r2.left);
                y0 = util::Max(r1.top, r2.top);
                x1 = util::Min(r1.right, r2.right);
                y1 = util::Min(r1.bottom, r2.bottom);
                break;
            }
            case Opcode::Difference: {
                return opr1_->GetDrawArea();
            }
            case Opcode::Rotate: {
                auto area = opr1_->GetDrawArea();
                float tx0 = area.left, ty0 = area.top;
                float tx1 = area.right, ty1 = area.top;
                float tx2 = area.right, ty2 = area.bottom;
                float tx3 = area.left, ty3 = area.bottom;
                CoordMapping(tx0, ty0, true);
                CoordMapping(tx1, ty1, true);
                CoordMapping(tx2, ty2, true);
                CoordMapping(tx3, ty3, true);
                x0 = std::floorf(util::Min(tx0, tx1, tx2, tx3));
                y0 = std::floorf(util::Min(ty0, ty1, ty2, ty3));
                x1 = std::ceilf(util::Max(tx0, tx1, tx2, tx3));
                y1 = std::ceilf(util::Max(ty0, ty1, ty2, ty3));
                break;
            }
            case Opcode::Scale: {
                auto area = opr1_->GetDrawArea();
                float tx0 = area.left, ty0 = area.top;
                float tx1 = area.right, ty1 = area.bottom;
                CoordMapping(tx0, ty0, true);
                CoordMapping(tx1, ty1, true);
                x0 = std::floorf(tx0);
                y0 = std::floorf(ty0);
                x1 = std::ceilf(tx1);
                y1 = std::ceilf(ty1);
                break;
            }
            case Opcode::Round: {
                auto area = opr1_->GetDrawArea();
                float tx0 = area.left, ty0 = area.top;
                float tx1 = area.right, ty1 = area.bottom;
                tx0 -= param_;
                ty0 -= param_;
                tx1 += param_;
                ty1 += param_;
                x0 = std::floorf(tx0);
                y0 = std::floorf(ty0);
                x1 = std::ceilf(tx1);
                y1 = std::ceilf(ty1);
                break;
            }
        }
        return Rect(x0, y0, x1, y1);
    }

private:
    // !reverse: processed -> orignal
    //  reverse: orignal   -> processed
    void CoordMapping(float &x, float &y, bool reverse = false) const {
        auto x1 = x - center_x_, y1 = y - center_y_;
        switch (opcode_) {
            case Opcode::Rotate: {
                auto rho = std::sqrtf(x1 * x1 + y1 * y1);
                float theta = std::atan2f(y1, x1);
                theta += reverse ? param_ : -param_;
                x = rho * std::cosf(theta) + center_x_;
                y = rho * std::sinf(theta) + center_y_;
                break;
            }
            case Opcode::Scale: {
                if (reverse) {
                    x = x1 * param_ + center_x_;
                    y = y1 * param_ + center_y_;
                }
                else {
                    x = x1 / param_ + center_x_;
                    y = y1 / param_ + center_y_;
                }
                break;
            }
            default:;
        }
    }

    Opcode opcode_;
    ShapePtr opr1_, opr2_;
    float center_x_, center_y_, param_;
};

} // namespace cvf::shape

#endif // CANVASFLAT_SHAPE_OPERATION_H_
