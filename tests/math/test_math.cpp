#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <aryabhatta/math/math.hpp>
#include <glm/gtc/constants.hpp>

using namespace aryabhatta::math;
using Catch::Approx;

TEST_CASE("Transform defaults to identity", "[math][transform]") {
  const Transform value{};

  REQUIRE(value.translation.x == Approx(0.0f));
  REQUIRE(value.translation.y == Approx(0.0f));
  REQUIRE(value.translation.z == Approx(0.0f));

  REQUIRE(value.rotation.w == Approx(1.0f));
  REQUIRE(value.rotation.x == Approx(0.0f));
  REQUIRE(value.rotation.y == Approx(0.0f));
  REQUIRE(value.rotation.z == Approx(0.0f));

  REQUIRE(value.scale.x == Approx(1.0f));
  REQUIRE(value.scale.y == Approx(1.0f));
  REQUIRE(value.scale.z == Approx(1.0f));

  const Mat4 matrix = value.to_mat4();
  for (int column = 0; column < 4; ++column) {
    for (int row = 0; row < 4; ++row) {
      REQUIRE(matrix[column][row] == Approx(column == row ? 1.0f : 0.0f));
    }
  }
}

TEST_CASE("Transform composes translation rotation and scale as T*R*S",
          "[math][transform]") {
  Transform value;
  value.translation = Vec3{10.0f, 20.0f, 30.0f};
  value.rotation = angle_axis(glm::radians(90.0f), UNIT_Y);
  value.scale = Vec3{2.0f};

  const Vec4 result = value.to_mat4() * Vec4{1.0f, 0.0f, 0.0f, 1.0f};

  REQUIRE(result.x == Approx(10.0f).margin(0.001f));
  REQUIRE(result.y == Approx(20.0f).margin(0.001f));
  REQUIRE(result.z == Approx(28.0f).margin(0.001f));
  REQUIRE(result.w == Approx(1.0f));
}

TEST_CASE("Coordinate basis is right-handed", "[math][conventions]") {
  const Vec3 result = glm::cross(UNIT_X, UNIT_Y);

  REQUIRE(result.x == Approx(UNIT_Z.x));
  REQUIRE(result.y == Approx(UNIT_Z.y));
  REQUIRE(result.z == Approx(UNIT_Z.z));
}

TEST_CASE("Translation and scale helpers transform homogeneous points",
          "[math][helpers]") {
  const Vec4 point{1.0f, 2.0f, 3.0f, 1.0f};

  const Vec4 moved = translate(Vec3{2.0f, 3.0f, 4.0f}) * point;
  REQUIRE(moved.x == Approx(3.0f));
  REQUIRE(moved.y == Approx(5.0f));
  REQUIRE(moved.z == Approx(7.0f));
  REQUIRE(moved.w == Approx(1.0f));

  const Vec4 scaled = scale(Vec3{2.0f, 3.0f, 4.0f}) * point;
  REQUIRE(scaled.x == Approx(2.0f));
  REQUIRE(scaled.y == Approx(6.0f));
  REQUIRE(scaled.z == Approx(12.0f));
  REQUIRE(scaled.w == Approx(1.0f));
}

TEST_CASE("Rotation helper rotates around the requested axis",
          "[math][helpers][rotation]") {
  const Vec4 point{1.0f, 0.0f, 0.0f, 1.0f};
  const Vec4 result = rotate(glm::half_pi<float>(), UNIT_Z) * point;

  REQUIRE(result.x == Approx(0.0f).margin(0.0001f));
  REQUIRE(result.y == Approx(1.0f).margin(0.0001f));
  REQUIRE(result.z == Approx(0.0f).margin(0.0001f));
  REQUIRE(result.w == Approx(1.0f));
}

TEST_CASE("Vector lerp interpolates endpoints and midpoint",
          "[math][interpolation]") {
  const Vec3 a{0.0f, 2.0f, 4.0f};
  const Vec3 b{10.0f, 12.0f, 14.0f};

  const Vec3 start = lerp(a, b, 0.0f);
  const Vec3 midpoint = lerp(a, b, 0.5f);
  const Vec3 finish = lerp(a, b, 1.0f);

  REQUIRE(start.x == Approx(a.x));
  REQUIRE(start.y == Approx(a.y));
  REQUIRE(start.z == Approx(a.z));

  REQUIRE(midpoint.x == Approx(5.0f));
  REQUIRE(midpoint.y == Approx(7.0f));
  REQUIRE(midpoint.z == Approx(9.0f));

  REQUIRE(finish.x == Approx(b.x));
  REQUIRE(finish.y == Approx(b.y));
  REQUIRE(finish.z == Approx(b.z));
}

TEST_CASE("Quaternion slerp produces normalized intermediate rotations",
          "[math][interpolation][quaternion]") {
  const Quat start = angle_axis(0.0f, UNIT_Z);
  const Quat finish = angle_axis(glm::half_pi<float>(), UNIT_Z);
  const Quat midpoint = slerp(start, finish, 0.5f);

  const Vec3 rotated = midpoint * UNIT_X;

  REQUIRE(glm::length(midpoint) == Approx(1.0f).margin(0.0001f));
  REQUIRE(rotated.x == Approx(glm::sqrt(0.5f)).margin(0.0001f));
  REQUIRE(rotated.y == Approx(glm::sqrt(0.5f)).margin(0.0001f));
}

TEST_CASE("Transform interpolation combines translation scale and rotation",
          "[math][interpolation][transform]") {
  Transform a;
  Transform b;

  a.translation = Vec3{0.0f};
  b.translation = Vec3{10.0f};

  a.scale = Vec3{1.0f};
  b.scale = Vec3{3.0f};

  b.rotation = angle_axis(glm::half_pi<float>(), UNIT_Z);

  const Transform midpoint = lerp(a, b, 0.5f);

  REQUIRE(midpoint.translation.x == Approx(5.0f));
  REQUIRE(midpoint.translation.y == Approx(5.0f));
  REQUIRE(midpoint.translation.z == Approx(5.0f));

  REQUIRE(midpoint.scale.x == Approx(2.0f));
  REQUIRE(midpoint.scale.y == Approx(2.0f));
  REQUIRE(midpoint.scale.z == Approx(2.0f));

  REQUIRE(glm::length(midpoint.rotation) == Approx(1.0f).margin(0.0001f));
}

TEST_CASE("Quaternion identity preserves a vector", "[math][quaternion]") {
  const Vec3 result = IDENTITY_QUAT * Vec3{1.0f, 2.0f, 3.0f};

  REQUIRE(result.x == Approx(1.0f));
  REQUIRE(result.y == Approx(2.0f));
  REQUIRE(result.z == Approx(3.0f));
}
