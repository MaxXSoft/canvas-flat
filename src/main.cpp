#include <iostream>
#include <ctime>
#include <cstdlib>

#include "canvas.h"
#include "container/pngcont.h"

#include "shape/circle.h"
#include "shape/squircle.h"
#include "shape/rectangle.h"
#include "shape/capsule.h"
#include "shape/operation.h"

Color GetRandColor(bool greyscale = false, bool opaque = false) {
    auto alpha = opaque ? 1.F : (std::rand() % 100 + 1) / 100.F;
    if (greyscale) {
        Color::Color8b grey = std::rand() & 0xFF;
        return Color((grey << 16) | (grey << 8) | grey, alpha);
    }
    else {
        return Color(std::rand() & 0xFFFFFF, alpha);
    }
}

int main(int argc, const char *argv[]) {
    // initialize random generator
    std::srand(std::time(nullptr));
    // create canvas
    Canvas<PngContainer> canvas(512, 512);
    canvas.set_backcolor(GetRandColor(true));
    canvas.set_anti_aliasing(true);
    // add shapes to canvas
    auto s1 = std::make_shared<Capsule>(100, 100, 100, 300, 20);
    auto s2 = std::make_shared<Operation>(Operation::Opcode::Rotate, s1, M_PI_4);
    canvas.AddShape(std::make_shared<Operation>(Operation::Opcode::Union, s1, s2));
    // canvas.AddShape(s2);
    // set color
    for (const auto &shape : canvas.shapes()) {
        shape->set_color(GetRandColor(true));
    }
    // draw & export
    canvas.Redraw();
    canvas.Export();
    return 0;
}
