#pragma once

// Aryabhatta - shared math foundation.
// Include this header before using GLM anywhere in a consuming project.
// These compile-time GLM conventions are part of the public ABI/configuration
// contract and must be consistent across every translation unit.

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#ifndef GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#endif
#ifndef GLM_FORCE_XYZW_ONLY
#define GLM_FORCE_XYZW_ONLY
#endif
#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

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

struct Transform {
    Vec3 translation{0.0f};
    Quat rotation{1.0f, 0.0f, 0.0f, 0.0f};
    Vec3 scale{1.0f};

    [[nodiscard]] Mat4 to_mat4() const {
        const Mat4 t = glm::translate(Mat4{1.0f}, translation);
        const Mat4 r = glm::mat4_cast(rotation);
        const Mat4 s = glm::scale(Mat4{1.0f}, scale);
        return t * r * s;
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
    return glm::rotate(Mat4{1.0f}, radians, axis);
}

[[nodiscard]] inline Quat angle_axis(float radians, const Vec3& axis) {
    return glm::angleAxis(radians, axis);
}

[[nodiscard]] inline Vec3 lerp(const Vec3& a, const Vec3& b, float t) {
    return glm::mix(a, b, t);
}

[[nodiscard]] inline Quat slerp(const Quat& a, const Quat& b, float t) {
    return glm::slerp(a, b, t);
}

[[nodiscard]] inline Transform lerp(const Transform& a, const Transform& b, float t) {
    return Transform{
        .translation = lerp(a.translation, b.translation, t),
        .rotation = slerp(a.rotation, b.rotation, t),
        .scale = lerp(a.scale, b.scale, t),
    };
}

} // namespace aryabhatta::math
