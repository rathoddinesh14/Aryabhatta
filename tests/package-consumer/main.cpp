#include <aryabhatta/math/math.hpp>
using namespace aryabhatta::math;

int main() {

    const Vec3 position{1.0f, 2.0f, 3.0f};

    const Mat4 translation = translate(position);

    const Mat4 rotation = rotate(glm::radians(90.0f), UNIT_Y);

    const Transform transform{.translation = position,
                              .rotation = angle_axis(glm::radians(90.0f), UNIT_Y),
                              .scale = Vec3{1.0f}};

    const Mat4 model = transform.to_mat4();

    (void)translation;
    (void)rotation;
    (void)model;

    return 0;
}