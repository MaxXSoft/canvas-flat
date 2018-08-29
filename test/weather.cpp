#include "../src/render/basic.h"
#include "../src/canvas.h"
#include "../src/container/pngcont.h"
#include "../src/util/mathutil.h"

#include "../src/shape/rectangle.h"
#include "../src/shape/operation.h"
#include "../src/shape/circle.h"
#include "../src/shape/capsule.h"

using namespace cvf;
using namespace cvf::render;
using namespace cvf::container;
using namespace cvf::color;
using namespace cvf::util;
using namespace cvf::shape;

int main(int argc, const char *argv[]) {
    // create render
    auto render = std::make_unique<BasicRender>();
    render->set_anti_aliasing(true);
    // create canvas
    Canvas canvas(1024, 1024);
    canvas.set_backcolor(0xFFFFFF);
    canvas.set_render(std::move(render));
    canvas.set_image_container(std::make_unique<PngContainer>());
    // create icon background
    auto cx = canvas.width() / 2, cy = canvas.height() / 2;
    auto round_r = 180.F, org_r = Min(cx, cy) * 3.F / 4.F;
    auto r = org_r - round_r;
    ShapePtr bg = std::make_shared<Rectangle>
            (cx - r, cy - r, r * 2);
    bg = std::make_shared<Operation>
            (Operation::Opcode::Round, bg, round_r);
    bg->set_color(Color(0x0278E2, 0x78EEFCU));
    // create shadow
    auto shadow = std::make_shared<Operation>
            (Operation::Opcode::Blur, bg, 300);
    shadow = std::make_shared<Operation>
            (Operation::Opcode::Scale, shadow, 1.2);
    shadow = std::make_shared<Operation>
            (Operation::Opcode::OffsetY, shadow, 30);
    shadow->set_color(Color(0, 0.3F));
    // create sun pattern
    auto sun = std::make_shared<Circle>
            (cx - org_r * 0.35, cy - org_r * 0.2, org_r * 0.37);
    sun->set_color(Color(0xFBC036, 0xF4E82DU));
    // create cloud pattern
    auto c1 = std::make_shared<Capsule>
            (cx - org_r * 0.32, cy + org_r * 0.23,
            cx + org_r * 0.38, cy + org_r * 0.23, org_r * 0.23);
    auto c2 = std::make_shared<Circle>
            (cx + org_r * 0.38, cy + org_r * 0.16, org_r * 0.3);
    auto c3 = std::make_shared<Circle>
            (cx, cy, org_r * 0.37);
    auto cloud = std::make_shared<Operation>
            (Operation::Opcode::Union, c1, c2);
    cloud = std::make_shared<Operation>
            (Operation::Opcode::Union, cloud, c3);
    cloud->set_color(Color(SolidColor(0xFFFFFF, 0.75),
            SolidColor(0xFFFFFF, 0.97)));
    // add shapes
    canvas.AddShape(shadow);
    canvas.AddShape(bg);
    canvas.AddShape(sun);
    canvas.AddShape(cloud);
    // draw & export
    canvas.Redraw();
    canvas.Export(argc > 1 ? argv[1] : "out/weather.png");
    return 0;
}
