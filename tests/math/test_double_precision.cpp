#include <aryabhatta/math/math.hpp>
#include <glm/gtc/constants.hpp>
#include <limits>
#include <type_traits>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace aryabhatta::math;
using Catch::Approx;

// -----------------------------------------------------------------------------
// Compile-time type checks
// -----------------------------------------------------------------------------

static_assert(std::is_same_v<Vec2, glm::vec2>);
static_assert(std::is_same_v<Vec3, glm::vec3>);
static_assert(std::is_same_v<Vec4, glm::vec4>);
static_assert(std::is_same_v<Mat3, glm::mat3>);
static_assert(std::is_same_v<Mat4, glm::mat4>);
static_assert(std::is_same_v<Quat, glm::quat>);

static_assert(std::is_same_v<DVec2, glm::dvec2>);
static_assert(std::is_same_v<DVec3, glm::dvec3>);
static_assert(std::is_same_v<DVec4, glm::dvec4>);
static_assert(std::is_same_v<DMat3, glm::dmat3>);
static_assert(std::is_same_v<DMat4, glm::dmat4>);
static_assert(std::is_same_v<DQuat, glm::dquat>);

static_assert(std::is_same_v<Transform, BasicTransform<float>>);
static_assert(std::is_same_v<DTransform, BasicTransform<double>>);

static_assert(std::is_same_v<decltype(lerp(DVec3{}, DVec3{}, 0.5)), DVec3>);

static_assert(std::is_same_v<decltype(slerp(DQuat{}, DQuat{}, 0.5)), DQuat>);

static_assert(std::is_same_v<decltype(translate(DVec3{})), DMat4>);

static_assert(std::is_same_v<decltype(rotate(0.5, DVec3{1.0, 0.0, 0.0})), DMat4>);

// -----------------------------------------------------------------------------
// Aliases and constants
// -----------------------------------------------------------------------------

TEST_CASE("Double precision constants have the expected values", "[math][double][constants]") {
    REQUIRE(DUNIT_X.x == Approx(1.0));
    REQUIRE(DUNIT_X.y == Approx(0.0));
    REQUIRE(DUNIT_X.z == Approx(0.0));

    REQUIRE(DUNIT_Y.x == Approx(0.0));
    REQUIRE(DUNIT_Y.y == Approx(1.0));
    REQUIRE(DUNIT_Y.z == Approx(0.0));

    REQUIRE(DUNIT_Z.x == Approx(0.0));
    REQUIRE(DUNIT_Z.y == Approx(0.0));
    REQUIRE(DUNIT_Z.z == Approx(1.0));

    REQUIRE(DIDENTITY_QUAT.w == Approx(1.0));
    REQUIRE(DIDENTITY_QUAT.x == Approx(0.0));
    REQUIRE(DIDENTITY_QUAT.y == Approx(0.0));
    REQUIRE(DIDENTITY_QUAT.z == Approx(0.0));
}

// -----------------------------------------------------------------------------
// Vector operations and precision
// -----------------------------------------------------------------------------

TEST_CASE("Double vectors support construction and arithmetic", "[math][double][vector]") {
    const DVec3 a{1.0, 2.0, 3.0};
    const DVec3 b{4.0, 5.0, 6.0};

    const DVec3 sum = a + b;
    const DVec3 difference = b - a;
    const DVec3 product = a * 2.0;

    REQUIRE(sum.x == Approx(5.0));
    REQUIRE(sum.y == Approx(7.0));
    REQUIRE(sum.z == Approx(9.0));

    REQUIRE(difference.x == Approx(3.0));
    REQUIRE(difference.y == Approx(3.0));
    REQUIRE(difference.z == Approx(3.0));

    REQUIRE(product.x == Approx(2.0));
    REQUIRE(product.y == Approx(4.0));
    REQUIRE(product.z == Approx(6.0));
}

TEST_CASE("Double vectors support dot cross length and normalization", "[math][double][vector]") {
    const DVec3 x{1.0, 0.0, 0.0};
    const DVec3 y{0.0, 1.0, 0.0};

    REQUIRE(glm::dot(x, y) == Approx(0.0));

    const DVec3 cross = glm::cross(x, y);

    REQUIRE(cross.x == Approx(0.0));
    REQUIRE(cross.y == Approx(0.0));
    REQUIRE(cross.z == Approx(1.0));

    const DVec3 value{2.0, 3.0, 6.0};

    REQUIRE(glm::length(value) == Approx(7.0));

    const DVec3 normalized = glm::normalize(value);

    REQUIRE(glm::length(normalized) == Approx(1.0).margin(1e-12));
}

TEST_CASE("Double vectors retain precision beyond float precision", "[math][double][precision]") {
    const double value = 1.0 + 1e-10;

    const DVec3 double_vector{value, value, value};
    const Vec3 float_vector{static_cast<float>(value), static_cast<float>(value),
                            static_cast<float>(value)};

    REQUIRE(double_vector.x == value);
    REQUIRE(double_vector.x != static_cast<double>(float_vector.x));
}

// -----------------------------------------------------------------------------
// Matrix operations
// -----------------------------------------------------------------------------

TEST_CASE("Double translation and scale helpers transform points", "[math][double][matrix]") {
    const DVec4 point{1.0, 2.0, 3.0, 1.0};

    const DVec4 moved = translate(DVec3{2.0, 3.0, 4.0}) * point;

    REQUIRE(moved.x == Approx(3.0));
    REQUIRE(moved.y == Approx(5.0));
    REQUIRE(moved.z == Approx(7.0));
    REQUIRE(moved.w == Approx(1.0));

    const DVec4 scaled = scale(DVec3{2.0, 3.0, 4.0}) * point;

    REQUIRE(scaled.x == Approx(2.0));
    REQUIRE(scaled.y == Approx(6.0));
    REQUIRE(scaled.z == Approx(12.0));
    REQUIRE(scaled.w == Approx(1.0));
}

TEST_CASE("Double matrices preserve identity", "[math][double][matrix]") {
    const DMat4 identity{1.0};

    for (int column = 0; column < 4; ++column) {
        for (int row = 0; row < 4; ++row) {
            REQUIRE(identity[column][row] == Approx(column == row ? 1.0 : 0.0));
        }
    }
}

TEST_CASE("Double matrix multiplication preserves large values", "[math][double][precision]") {
    const double large_value = 1e12;

    const DMat4 translation = translate(DVec3{large_value, 0.0, 0.0});

    const DVec4 point{1.0, 0.0, 0.0, 1.0};
    const DVec4 result = translation * point;

    REQUIRE(result.x == Approx(large_value + 1.0));
    REQUIRE(result.y == Approx(0.0));
    REQUIRE(result.z == Approx(0.0));
    REQUIRE(result.w == Approx(1.0));
}

// -----------------------------------------------------------------------------
// Rotation and quaternion operations
// -----------------------------------------------------------------------------

TEST_CASE("Double angle-axis rotation rotates vectors correctly", "[math][double][quaternion]") {
    const double angle = glm::half_pi<double>();

    const DQuat rotation = angle_axis(angle, DUNIT_Z);
    const DVec3 result = rotation * DUNIT_X;

    REQUIRE(glm::length(rotation) == Approx(1.0).margin(1e-12));
    REQUIRE(result.x == Approx(0.0).margin(1e-12));
    REQUIRE(result.y == Approx(1.0).margin(1e-12));
    REQUIRE(result.z == Approx(0.0).margin(1e-12));
}

TEST_CASE("Double rotation matrix rotates vectors correctly", "[math][double][rotation]") {
    const DMat4 rotation = rotate(glm::half_pi<double>(), DUNIT_Z);

    const DVec4 result = rotation * DVec4{1.0, 0.0, 0.0, 1.0};

    REQUIRE(result.x == Approx(0.0).margin(1e-12));
    REQUIRE(result.y == Approx(1.0).margin(1e-12));
    REQUIRE(result.z == Approx(0.0).margin(1e-12));
    REQUIRE(result.w == Approx(1.0));
}

TEST_CASE("Double rotation normalizes non-unit axes", "[math][double][rotation]") {
    const double angle = glm::half_pi<double>();

    const DQuat unit_axis = angle_axis(angle, DVec3{0.0, 1.0, 0.0});

    const DQuat non_unit_axis = angle_axis(angle, DVec3{0.0, 5.0, 0.0});

    REQUIRE(glm::abs(glm::dot(unit_axis, non_unit_axis)) == Approx(1.0).margin(1e-12));
}

TEST_CASE("Double rotation rejects zero and non-finite axes", "[math][double][rotation]") {
    const double angle = glm::half_pi<double>();

    REQUIRE_THROWS_AS(rotate(angle, DVec3{0.0, 0.0, 0.0}), std::invalid_argument);

    REQUIRE_THROWS_AS(angle_axis(angle, DVec3{std::numeric_limits<double>::infinity(), 0.0, 0.0}),
                      std::invalid_argument);

    REQUIRE_THROWS_AS(rotate(angle, DVec3{std::numeric_limits<double>::quiet_NaN(), 0.0, 0.0}),
                      std::invalid_argument);
}

TEST_CASE("Double rotation normalizes very large and very small axes",
          "[math][double][rotation][precision]") {
    const double angle = glm::half_pi<double>();

    const DQuat reference = angle_axis(angle, DUNIT_Y);

    const DQuat large_axis = angle_axis(angle, DVec3{0.0, 1e300, 0.0});

    const DQuat small_axis = angle_axis(angle, DVec3{0.0, 1e-300, 0.0});

    REQUIRE(glm::abs(glm::dot(reference, large_axis)) == Approx(1.0).margin(1e-12));

    REQUIRE(glm::abs(glm::dot(reference, small_axis)) == Approx(1.0).margin(1e-12));
}

// -----------------------------------------------------------------------------
// Quaternion interpolation
// -----------------------------------------------------------------------------

TEST_CASE("Double quaternion slerp interpolates rotations",
          "[math][double][interpolation][quaternion]") {
    const DQuat start = angle_axis(0.0, DUNIT_Z);
    const DQuat finish = angle_axis(glm::half_pi<double>(), DUNIT_Z);

    const DQuat midpoint = slerp(start, finish, 0.5);

    const DVec3 rotated = midpoint * DUNIT_X;

    const double expected = glm::sqrt(0.5);

    REQUIRE(glm::length(midpoint) == Approx(1.0).margin(1e-12));
    REQUIRE(rotated.x == Approx(expected).margin(1e-12));
    REQUIRE(rotated.y == Approx(expected).margin(1e-12));
}

TEST_CASE("Double quaternion slerp preserves endpoints",
          "[math][double][interpolation][quaternion]") {
    const DQuat start = DIDENTITY_QUAT;
    const DQuat finish = angle_axis(glm::pi<double>(), DUNIT_Y);

    const DQuat first = slerp(start, finish, 0.0);
    const DQuat last = slerp(start, finish, 1.0);

    REQUIRE(glm::abs(glm::dot(first, start)) == Approx(1.0).margin(1e-12));

    REQUIRE(glm::abs(glm::dot(last, finish)) == Approx(1.0).margin(1e-12));
}

// -----------------------------------------------------------------------------
// Vector interpolation
// -----------------------------------------------------------------------------

TEST_CASE("Double vector lerp interpolates endpoints and midpoint",
          "[math][double][interpolation]") {
    const DVec3 a{0.0, 2.0, 4.0};
    const DVec3 b{10.0, 12.0, 14.0};

    const DVec3 start = lerp(a, b, 0.0);
    const DVec3 midpoint = lerp(a, b, 0.5);
    const DVec3 finish = lerp(a, b, 1.0);

    REQUIRE(start.x == Approx(a.x));
    REQUIRE(start.y == Approx(a.y));
    REQUIRE(start.z == Approx(a.z));

    REQUIRE(midpoint.x == Approx(5.0));
    REQUIRE(midpoint.y == Approx(7.0));
    REQUIRE(midpoint.z == Approx(9.0));

    REQUIRE(finish.x == Approx(b.x));
    REQUIRE(finish.y == Approx(b.y));
    REQUIRE(finish.z == Approx(b.z));
}

// -----------------------------------------------------------------------------
// DTransform
// -----------------------------------------------------------------------------

TEST_CASE("DTransform defaults to identity", "[math][double][transform]") {
    const DTransform transform{};

    REQUIRE(transform.translation.x == Approx(0.0));
    REQUIRE(transform.translation.y == Approx(0.0));
    REQUIRE(transform.translation.z == Approx(0.0));

    REQUIRE(transform.rotation.w == Approx(1.0));
    REQUIRE(transform.rotation.x == Approx(0.0));
    REQUIRE(transform.rotation.y == Approx(0.0));
    REQUIRE(transform.rotation.z == Approx(0.0));

    REQUIRE(transform.scale.x == Approx(1.0));
    REQUIRE(transform.scale.y == Approx(1.0));
    REQUIRE(transform.scale.z == Approx(1.0));

    const DMat4 matrix = transform.to_mat4();

    for (int column = 0; column < 4; ++column) {
        for (int row = 0; row < 4; ++row) {
            REQUIRE(matrix[column][row] == Approx(column == row ? 1.0 : 0.0));
        }
    }
}

TEST_CASE("DTransform identity factory returns identity", "[math][double][transform]") {
    const DTransform transform = DTransform::identity();

    REQUIRE(transform.translation == DVec3{0.0});
    REQUIRE(transform.rotation.w == Approx(1.0));
    REQUIRE(transform.scale == DVec3{1.0});
}

TEST_CASE("DTransform composes translation rotation and scale as T*R*S",
          "[math][double][transform]") {
    DTransform transform;

    transform.translation = DVec3{10.0, 20.0, 30.0};
    transform.rotation = angle_axis(glm::half_pi<double>(), DUNIT_Y);
    transform.scale = DVec3{2.0};

    const DVec4 result = transform.to_mat4() * DVec4{1.0, 0.0, 0.0, 1.0};

    REQUIRE(result.x == Approx(10.0).margin(1e-12));
    REQUIRE(result.y == Approx(20.0).margin(1e-12));
    REQUIRE(result.z == Approx(28.0).margin(1e-12));
    REQUIRE(result.w == Approx(1.0));
}

TEST_CASE("DTransform preserves double precision in translation",
          "[math][double][transform][precision]") {
    DTransform transform;

    transform.translation = DVec3{1e12 + 1.0, 0.0, 0.0};

    const DVec4 result = transform.to_mat4() * DVec4{0.0, 0.0, 0.0, 1.0};

    REQUIRE(result.x == Approx(1e12 + 1.0));
    REQUIRE(result.x != static_cast<double>(static_cast<float>(result.x)));
}

TEST_CASE("DTransform interpolation combines translation scale and rotation",
          "[math][double][transform][interpolation]") {
    DTransform a;
    DTransform b;

    a.translation = DVec3{0.0};
    b.translation = DVec3{10.0};

    a.scale = DVec3{1.0};
    b.scale = DVec3{3.0};

    b.rotation = angle_axis(glm::half_pi<double>(), DUNIT_Z);

    const DTransform midpoint = lerp(a, b, 0.5);

    REQUIRE(midpoint.translation.x == Approx(5.0));
    REQUIRE(midpoint.translation.y == Approx(5.0));
    REQUIRE(midpoint.translation.z == Approx(5.0));

    REQUIRE(midpoint.scale.x == Approx(2.0));
    REQUIRE(midpoint.scale.y == Approx(2.0));
    REQUIRE(midpoint.scale.z == Approx(2.0));

    REQUIRE(glm::length(midpoint.rotation) == Approx(1.0).margin(1e-12));
}