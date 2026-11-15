#pragma once

// Must be defined before including GLM experimental extensions.
#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <stdexcept>

namespace aryabhatta::math {

using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;

using Mat3 = glm::mat3;
using Mat4 = glm::mat4;

using Quat = glm::quat;

inline const Vec3 UNIT_X{1.0f, 0.0f, 0.0f};
inline const Vec3 UNIT_Y{0.0f, 1.0f, 0.0f};
inline const Vec3 UNIT_Z{0.0f, 0.0f, 1.0f};

inline const Quat IDENTITY_QUAT{1.0f, 0.0f, 0.0f, 0.0f};

namespace detail {

// GLM rotation functions expect a normalized axis.
// Reject invalid axes instead of producing undefined numerical results.
[[nodiscard]] inline Vec3 normalized_axis(const Vec3& axis) {
    const float axis_length = glm::length(axis);

    if (!std::isfinite(axis_length) || axis_length <= 1e-6f) {
        throw std::invalid_argument("Rotation axis must be a finite, non-zero vector.");
    }

    return axis / axis_length;
}

} // namespace detail

struct Transform {
    Vec3 translation{0.0f};
    Quat rotation{1.0f, 0.0f, 0.0f, 0.0f};
    Vec3 scale{1.0f};

    [[nodiscard]] Mat4 to_mat4() const {
        const Mat4 translation_matrix = glm::translate(Mat4{1.0f}, translation);

        const Mat4 rotation_matrix = glm::mat4_cast(rotation);

        const Mat4 scale_matrix = glm::scale(Mat4{1.0f}, scale);

        return translation_matrix * rotation_matrix * scale_matrix;
    }

    [[nodiscard]] static Transform identity() { return {}; }
};

[[nodiscard]] inline Mat4 translate(const Vec3& value) {
    return glm::translate(Mat4{1.0f}, value);
}

[[nodiscard]] inline Mat4 scale(const Vec3& value) {
    return glm::scale(Mat4{1.0f}, value);
}

[[nodiscard]] inline Mat4 rotate(float radians, const Vec3& axis) {
    const Vec3 unit_axis = detail::normalized_axis(axis);
    return glm::rotate(Mat4{1.0f}, radians, unit_axis);
}

[[nodiscard]] inline Quat angle_axis(float radians, const Vec3& axis) {
    const Vec3 unit_axis = detail::normalized_axis(axis);
    return glm::angleAxis(radians, unit_axis);
}

[[nodiscard]] inline Vec3 lerp(const Vec3& a, const Vec3& b, float t) {
    return glm::mix(a, b, t);
}

[[nodiscard]] inline Quat slerp(const Quat& a, const Quat& b, float t) {
    return glm::slerp(a, b, t);
}

[[nodiscard]] inline Transform lerp(const Transform& a, const Transform& b, float t) {
    Transform result;
    result.translation = lerp(a.translation, b.translation, t);
    result.rotation = slerp(a.rotation, b.rotation, t);
    result.scale = lerp(a.scale, b.scale, t);
    return result;
}

} // namespace aryabhatta::math
