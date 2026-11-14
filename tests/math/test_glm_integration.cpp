#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <aryabhatta/math/math.hpp>
#include <glm/gtc/constants.hpp>

using namespace aryabhatta::math;
using Catch::Approx;

// These tests validate the public math conventions and GLM-backed API.

TEST_CASE("Matrices use column-major storage", "[math][glm][matrix]") {
  Mat4 matrix{1.0f};
  matrix[0] = Vec4{2.0f, 3.0f, 4.0f, 5.0f};

  REQUIRE(matrix[0][0] == Approx(2.0f));
  REQUIRE(matrix[0][1] == Approx(3.0f));
  REQUIRE(matrix[0][2] == Approx(4.0f));
  REQUIRE(matrix[0][3] == Approx(5.0f));
}

TEST_CASE("Quaternion identity uses scalar-first GLM representation",
          "[math][glm][quaternion]") {
  const Quat quaternion = IDENTITY_QUAT;

  REQUIRE(quaternion.w == Approx(1.0f));
  REQUIRE(quaternion.x == Approx(0.0f));
  REQUIRE(quaternion.y == Approx(0.0f));
  REQUIRE(quaternion.z == Approx(0.0f));
}

TEST_CASE("Coordinate system is right-handed", "[math][glm][coordinates]") {
  const Vec3 result = glm::cross(UNIT_X, UNIT_Y);

  REQUIRE(result.x == Approx(0.0f));
  REQUIRE(result.y == Approx(0.0f));
  REQUIRE(result.z == Approx(1.0f));
}

TEST_CASE("Transform produces T * R * S matrix", "[math][transform]") {
  Transform transform;
  transform.translation = Vec3{10.0f, 20.0f, 30.0f};
  transform.rotation = angle_axis(glm::radians(90.0f), UNIT_Y);
  transform.scale = Vec3{2.0f};

  const Vec4 local_point{1.0f, 0.0f, 0.0f, 1.0f};
  const Vec4 world_point = transform.to_mat4() * local_point;

  REQUIRE(world_point.x == Approx(10.0f).margin(0.001f));
  REQUIRE(world_point.y == Approx(20.0f).margin(0.001f));
  REQUIRE(world_point.z == Approx(28.0f).margin(0.001f));
}

TEST_CASE("Slerp of identical quaternions preserves the quaternion",
          "[math][quaternion][interpolation]") {
  const Quat value = angle_axis(glm::radians(45.0f), UNIT_Y);
  const Quat result = slerp(value, value, 0.5f);

  REQUIRE(result.w == Approx(value.w).margin(0.0001f));
  REQUIRE(result.x == Approx(value.x).margin(0.0001f));
  REQUIRE(result.y == Approx(value.y).margin(0.0001f));
  REQUIRE(result.z == Approx(value.z).margin(0.0001f));
}

TEST_CASE("Vector lerp is component-wise linear",
          "[math][vector][interpolation]") {
  const Vec3 a{0.0f, 0.0f, 0.0f};
  const Vec3 b{10.0f, 20.0f, 30.0f};
  const Vec3 midpoint = lerp(a, b, 0.5f);

  REQUIRE(midpoint.x == Approx(5.0f));
  REQUIRE(midpoint.y == Approx(10.0f));
  REQUIRE(midpoint.z == Approx(15.0f));
}

TEST_CASE("Transform identity factory produces identity matrix",
          "[math][transform]") {
  const Transform identity = Transform::identity();
  const Mat4 matrix = identity.to_mat4();

  for (int column = 0; column < 4; ++column) {
    for (int row = 0; row < 4; ++row) {
      REQUIRE(matrix[column][row] == Approx(column == row ? 1.0f : 0.0f));
    }
  }
}
