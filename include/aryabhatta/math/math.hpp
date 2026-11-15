#pragma once

// Must be defined before including GLM experimental extensions.
#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif

#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <stdexcept>
#include <type_traits>

namespace aryabhatta::math {

// -----------------------------------------------------------------------------
// Public scalar-specific GLM aliases
// -----------------------------------------------------------------------------

using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;

using Mat3 = glm::mat3;
using Mat4 = glm::mat4;

using Quat = glm::quat;

using DVec2 = glm::dvec2;
using DVec3 = glm::dvec3;
using DVec4 = glm::dvec4;

using DMat3 = glm::dmat3;
using DMat4 = glm::dmat4;

using DQuat = glm::dquat;

// -----------------------------------------------------------------------------
// Constants
// -----------------------------------------------------------------------------

inline const Vec3 UNIT_X{1.0f, 0.0f, 0.0f};
inline const Vec3 UNIT_Y{0.0f, 1.0f, 0.0f};
inline const Vec3 UNIT_Z{0.0f, 0.0f, 1.0f};

inline const Quat IDENTITY_QUAT{1.0f, 0.0f, 0.0f, 0.0f};

inline const DVec3 DUNIT_X{1.0, 0.0, 0.0};
inline const DVec3 DUNIT_Y{0.0, 1.0, 0.0};
inline const DVec3 DUNIT_Z{0.0, 0.0, 1.0};

inline const DQuat DIDENTITY_QUAT{1.0, 0.0, 0.0, 0.0};

// -----------------------------------------------------------------------------
// Generic type definitions
// -----------------------------------------------------------------------------

template <typename T>
using BasicVec2 = glm::vec<2, T, glm::defaultp>;

template <typename T>
using BasicVec3 = glm::vec<3, T, glm::defaultp>;

template <typename T>
using BasicVec4 = glm::vec<4, T, glm::defaultp>;

template <typename T>
using BasicMat3 = glm::mat<3, 3, T, glm::defaultp>;

template <typename T>
using BasicMat4 = glm::mat<4, 4, T, glm::defaultp>;

template <typename T>
using BasicQuat = glm::qua<T, glm::defaultp>;

// -----------------------------------------------------------------------------
// Internal numerical utilities
// -----------------------------------------------------------------------------

namespace detail {

template <typename T>
inline constexpr bool supported_scalar_v = std::is_same_v<T, float> || std::is_same_v<T, double>;

// Normalize an axis without directly calculating its original length.
//
// Scaling by the largest absolute component first avoids intermediate
// overflow for very large vectors and underflow for very small vectors.
template <typename T>
    requires supported_scalar_v<T>
[[nodiscard]] inline BasicVec3<T> normalized_axis(const BasicVec3<T>& axis) {
    if (!std::isfinite(axis.x) || !std::isfinite(axis.y) || !std::isfinite(axis.z)) {
        throw std::invalid_argument("Rotation axis must be a finite, non-zero vector.");
    }

    const T max_component =
        std::max(std::abs(axis.x), std::max(std::abs(axis.y), std::abs(axis.z)));

    if (max_component == T(0)) {
        throw std::invalid_argument("Rotation axis must be a finite, non-zero vector.");
    }

    const BasicVec3<T> scaled_axis = axis / max_component;
    const T scaled_length = glm::length(scaled_axis);

    if (!std::isfinite(scaled_length) || scaled_length == T(0)) {
        throw std::invalid_argument("Rotation axis must be a finite, non-zero vector.");
    }

    return scaled_axis / scaled_length;
}

} // namespace detail

// -----------------------------------------------------------------------------
// Generic transform
// -----------------------------------------------------------------------------

template <typename T>
    requires detail::supported_scalar_v<T>
struct BasicTransform {
    using Scalar = T;
    using Vec3Type = BasicVec3<T>;
    using QuatType = BasicQuat<T>;
    using Mat4Type = BasicMat4<T>;

    Vec3Type translation{T(0)};
    QuatType rotation{T(1), T(0), T(0), T(0)};
    Vec3Type scale{T(1)};

    [[nodiscard]] Mat4Type to_mat4() const {
        const Mat4Type translation_matrix = glm::translate(Mat4Type{T(1)}, translation);

        const Mat4Type rotation_matrix = glm::mat4_cast(rotation);

        const Mat4Type scale_matrix = glm::scale(Mat4Type{T(1)}, scale);

        return translation_matrix * rotation_matrix * scale_matrix;
    }

    [[nodiscard]] static BasicTransform identity() { return {}; }
};

// Preserve the existing float API and provide its double equivalent.
using Transform = BasicTransform<float>;
using DTransform = BasicTransform<double>;

// -----------------------------------------------------------------------------
// Generic transformation helpers
// -----------------------------------------------------------------------------

template <typename T>
    requires detail::supported_scalar_v<T>
[[nodiscard]] inline BasicMat4<T> translate(const BasicVec3<T>& value) {
    return glm::translate(BasicMat4<T>{T(1)}, value);
}

template <typename T>
    requires detail::supported_scalar_v<T>
[[nodiscard]] inline BasicMat4<T> scale(const BasicVec3<T>& value) {
    return glm::scale(BasicMat4<T>{T(1)}, value);
}

template <typename T>
    requires detail::supported_scalar_v<T>
[[nodiscard]] inline BasicMat4<T> rotate(T radians, const BasicVec3<T>& axis) {

    const BasicVec3<T> unit_axis = detail::normalized_axis(axis);

    return glm::rotate(BasicMat4<T>{T(1)}, radians, unit_axis);
}

template <typename T>
    requires detail::supported_scalar_v<T>
[[nodiscard]] inline BasicQuat<T> angle_axis(T radians, const BasicVec3<T>& axis) {

    const BasicVec3<T> unit_axis = detail::normalized_axis(axis);

    return glm::angleAxis(radians, unit_axis);
}

// -----------------------------------------------------------------------------
// Generic interpolation helpers
// -----------------------------------------------------------------------------

template <typename T>
    requires detail::supported_scalar_v<T>
[[nodiscard]] inline BasicVec3<T> lerp(const BasicVec3<T>& a, const BasicVec3<T>& b, T t) {

    return glm::mix(a, b, t);
}

template <typename T>
    requires detail::supported_scalar_v<T>
[[nodiscard]] inline BasicQuat<T> slerp(const BasicQuat<T>& a, const BasicQuat<T>& b, T t) {

    return glm::slerp(a, b, t);
}

template <typename T>
    requires detail::supported_scalar_v<T>
[[nodiscard]] inline BasicTransform<T> lerp(const BasicTransform<T>& a, const BasicTransform<T>& b,
                                            T t) {

    BasicTransform<T> result;

    result.translation = lerp(a.translation, b.translation, t);
    result.rotation = slerp(a.rotation, b.rotation, t);
    result.scale = lerp(a.scale, b.scale, t);

    return result;
}

} // namespace aryabhatta::math
