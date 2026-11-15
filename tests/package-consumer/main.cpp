#include <aryabhatta/math/math.hpp>
#include <type_traits>

using namespace aryabhatta::math;

static_assert(std::is_same_v<decltype(Vec3{}), glm::vec3>);
static_assert(std::is_same_v<decltype(DVec3{}), glm::dvec3>);
static_assert(std::is_same_v<decltype(Mat4{}), glm::mat4>);
static_assert(std::is_same_v<decltype(DMat4{}), glm::dmat4>);

int main() {
    // Float API
    const Vec3 position{1.0f, 2.0f, 3.0f};

    const Mat4 translation = translate(position);
    const Mat4 rotation = rotate(glm::radians(90.0f), UNIT_Y);

    const Transform transform{.translation = position,
                              .rotation = angle_axis(glm::radians(90.0f), UNIT_Y),
                              .scale = Vec3{1.0f}};

    const Mat4 model = transform.to_mat4();

    // Double API
    const DVec3 double_position{1.0, 2.0, 3.0};

    const DMat4 double_translation = translate(double_position);
    const DMat4 double_rotation = rotate(glm::radians(90.0), DUNIT_Y);

    const DTransform double_transform{.translation = double_position,
                                      .rotation = angle_axis(glm::radians(90.0), DUNIT_Y),
                                      .scale = DVec3{1.0}};

    const DMat4 double_model = double_transform.to_mat4();

    const DQuat double_rotation_quaternion =
        slerp(DIDENTITY_QUAT, angle_axis(glm::half_pi<double>(), DUNIT_Z), 0.5);

    const DVec3 double_interpolated = lerp(DVec3{0.0}, DVec3{10.0}, 0.5);

    (void)translation;
    (void)rotation;
    (void)model;

    (void)double_translation;
    (void)double_rotation;
    (void)double_model;
    (void)double_rotation_quaternion;
    (void)double_interpolated;

    return 0;
}