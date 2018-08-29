#ifndef CANVASFLAT_UTIL_MATHUTIL_H_
#define CANVASFLAT_UTIL_MATHUTIL_H_

#include <cmath>
#include <limits>

namespace cvf::util {

// definition of mathematical constants
constexpr float PI = 3.14159265358979323846264338327950288;
constexpr float PI_2 = 1.57079632679489661923132169163975144;
constexpr float PI_4 = 0.785398163397448309615660845819875721;
constexpr float E = 2.71828182845904523536028747135266250;

// convert functions
inline float Radians(float degrees) {
    return degrees * PI / 180;
}

inline float Degrees(float radians) {
    return radians * 180 / PI;
}

// definition of function template 'Min' & 'Max'
template <typename T>
inline T Min(T a, T b) {
    return a < b ? a : b;
}

template <>
inline float Min<float>(float a, float b) {
    return std::fminf(a, b);
}

template <typename T, typename... Rest>
inline T Min(T a, T b, Rest... rest) {
    return Min(Min(a, b), rest...);
}

template <typename T>
inline T Max(T a, T b) {
    return a > b ? a : b;
}

template <>
inline float Max<float>(float a, float b) {
    return std::fmaxf(a, b);
}

template <typename T, typename... Rest>
inline T Max(T a, T b, Rest... rest) {
    return Max(Max(a, b), rest...);
}

// relational operation
inline bool FloatEqual(float a, float b) {
    auto abs_a = std::fabsf(a), abs_b = std::fabsf(b);
    return std::fabsf(a - b) <= (abs_a < abs_b ? abs_b : abs_a)
            * std::numeric_limits<float>::epsilon();
}

// some other definitions
inline bool RadiansNormalize(float radians) {
    auto x = std::fmodf(radians, 2 * PI);
    return x < 0 ? x + 2 * PI : x;
}

inline float LinearMapping(float value,
        float l0, float r0, float l1, float r1) {
    if (value < l0) {
        return l1;
    }
    else if (value > r0) {
        return r1;
    }
    else {
        return (value - l0) * (r1 - l1) / (r0 - l0) + l1;
    }
}

} // namespace cvf::util

#endif // CANVASFLAT_UTIL_MATHUTIL_H_
