#include <iostream>

#include "../src/canvas.h"
#include "../src/container/pngcont.h"
#include "../src/util/mathutil.h"

#include "../src/shape/circle.h"
#include "../src/shape/squircle.h"
#include "../src/shape/rectangle.h"
#include "../src/shape/capsule.h"
#include "../src/shape/operation.h"

using namespace cvf;
using namespace cvf::container;
using namespace cvf::util;
using namespace cvf::shape;

int main(int argc, const char *argv[]) {
    // create canvas
    Canvas<PngContainer> canvas(512, 512);
    canvas.set_backcolor(0x666666);
    canvas.set_anti_aliasing(true);
    // create a squircle
    auto cx = canvas.width() / 2, cy = canvas.height() / 2;
    auto r = util::Min(cx, cy) * 3.F / 2.5F;
    auto s1 = std::make_shared<Squircle>(cx, cy, r);
    // generate canvas
    auto last_rgb = canvas.backcolor().GetRGB();
    ShapePtr temp = s1;
    while ((r *= 0.9) >= 10) {
        temp = std::make_shared<Operation>
                (Operation::Opcode::Scale, temp, 0.9);
        temp = std::make_shared<Operation>
                (Operation::Opcode::Rotate, temp, util::PI / 12);
        temp->set_color(last_rgb -= 0x123123);
        canvas.AddShape(temp);
    }
    // draw & export
    canvas.Redraw();
    canvas.Export(argc > 1 ? argv[1] : "export.png");
    return 0;
}
