#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <aryabhatta/math/math.hpp>
#include <glm/gtc/constants.hpp>

using namespace aryabhatta::math;
using Catch::Approx;

TEST_CASE("Transform defaults to identity", "[math][transform]") {
    const Transform value{};
    REQUIRE(value.translation == Vec3{0.0f});
    REQUIRE(value.rotation.w == Approx(1.0f));
    REQUIRE(value.scale == Vec3{1.0f});
    REQUIRE(value.to_mat4() == Mat4{1.0f});
}

TEST_CASE("Transform composes translation rotation and scale as T*R*S", "[math][transform]") {
    Transform value;
    value.translation = Vec3{10.0f, 20.0f, 30.0f};
    value.rotation = angle_axis(glm::radians(90.0f), UNIT_Y);
    value.scale = Vec3{2.0f};
    const Vec4 result = value.to_mat4() * Vec4{1.0f, 0.0f, 0.0f, 1.0f};
    REQUIRE(result.x == Approx(10.0f).margin(0.001f));
    REQUIRE(result.y == Approx(20.0f).margin(0.001f));
    REQUIRE(result.z == Approx(28.0f).margin(0.001f));
}

TEST_CASE("Coordinate system is right-handed", "[math][conventions]") {
    REQUIRE(glm::cross(UNIT_X, UNIT_Y) == UNIT_Z);
}

TEST_CASE("translate and scale helpers transform homogeneous points", "[math][helpers]") {
    const Vec4 point{1.0f, 2.0f, 3.0f, 1.0f};
    const Vec4 moved = translate(Vec3{2.0f, 3.0f, 4.0f}) * point;
    REQUIRE(moved == Vec4{3.0f, 5.0f, 7.0f, 1.0f});
    const Vec4 scaled = scale(Vec3{2.0f, 3.0f, 4.0f}) * point;
    REQUIRE(scaled == Vec4{2.0f, 6.0f, 12.0f, 1.0f});
}

TEST_CASE("Vector lerp interpolates endpoints and midpoint", "[math][interpolation]") {
    const Vec3 a{0.0f, 2.0f, 4.0f};
    const Vec3 b{10.0f, 12.0f, 14.0f};
    REQUIRE(lerp(a, b, 0.0f) == a);
    REQUIRE(lerp(a, b, 0.5f) == Vec3{5.0f, 7.0f, 9.0f});
    REQUIRE(lerp(a, b, 1.0f) == b);
}

TEST_CASE("Quaternion slerp produces normalized intermediate rotations", "[math][interpolation][quat]") {
    const Quat a = angle_axis(0.0f, UNIT_Z);
    const Quat b = angle_axis(glm::half_pi<float>(), UNIT_Z);
    const Quat midpoint = slerp(a, b, 0.5f);
    const Vec3 rotated = midpoint * UNIT_X;
    REQUIRE(glm::length(midpoint) == Approx(1.0f).margin(0.0001f));
    REQUIRE(rotated.x == Approx(glm::sqrt(0.5f)).margin(0.0001f));
    REQUIRE(rotated.y == Approx(glm::sqrt(0.5f)).margin(0.0001f));
}

TEST_CASE("Transform lerp interpolates translation scale and rotation", "[math][interpolation][transform]") {
    Transform a;
    Transform b;
    a.translation = Vec3{0.0f};
    b.translation = Vec3{10.0f};
    a.scale = Vec3{1.0f};
    b.scale = Vec3{3.0f};
    b.rotation = angle_axis(glm::half_pi<float>(), UNIT_Z);

    const Transform middle = lerp(a, b, 0.5f);
    REQUIRE(middle.translation == Vec3{5.0f});
    REQUIRE(middle.scale == Vec3{2.0f});
    REQUIRE(glm::length(middle.rotation) == Approx(1.0f).margin(0.0001f));
}
