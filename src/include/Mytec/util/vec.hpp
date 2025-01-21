#ifndef MYTEC_VECTORS_HPP
#define MYTEC_VECTORS_HPP
#include <algorithm>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>


namespace mytec
{
template <class T>
using vec2 = sf::Vector2<T>;

using v2i = vec2<int32_t>;
using v2f = vec2<float>;
using v2u = vec2<uint32_t>;

// TODO: PR for SFML to extend operator types

template <class T>
constexpr vec2<T> operator +(const vec2<T> a, const T v)
{
    return a + vec2{v, v};
}

template <class T>
constexpr vec2<T> operator-(const vec2<T> a, const T v)
{
    return {a.x - v, a.y - v};
}

template <class T>
constexpr vec2<T> operator-(const T v, const vec2<T> a)
{
    return vec2{v, v} - a;
}

template <class T>
constexpr vec2<T> operator*(const vec2<T> a, const vec2<T> b)
{
    return {a.x * b.x, a.y * b.y};
}

template <class T>
constexpr vec2<T> &operator*=(vec2<T> &a, const vec2<T> b)
{
    return a = a * b;
}

template <class T>
constexpr vec2<T> operator/(const vec2<T> a, const vec2<T> b)
{
    return {a.x / b.x, a.y / b.y};
}

template <class T>
constexpr vec2<T> &operator/=(vec2<T> &a, const vec2<T> b)
{
    return a = a / b;
}

template <class T>
constexpr vec2<T> clamp(const vec2<T> v, const vec2<T> min, const vec2<T> max)
{
    return {std::clamp(v.x, min.x, max.x), std::clamp(v.y, min.y, max.y)};
}

template <class T>
constexpr vec2<T> operator<(const vec2<T> a, const T b)
{
    return {static_cast<T>(a.x < b), static_cast<T>(a.y < b)};
}

template <class T>
constexpr vec2<T> operator>(const vec2<T> a, const T b)
{
    return {static_cast<T>(a.x > b), (a.y > b)};
}

template <class T>
constexpr vec2<T> operator<=(const vec2<T> a, const T b)
{
    return {static_cast<T>(a.x <= b), static_cast<T>(a.y <= b)};
}

template <class T>
constexpr vec2<T> operator>=(const vec2<T> a, const T b)
{
    return {static_cast<T>(a.x >= b), static_cast<T>(a.y >= b)};
}

template <class T>
constexpr vec2<T> operator==(const vec2<T> a, const T b)
{
    return {static_cast<T>(a.x == b), static_cast<T>(a.y == b)};
}

template <class T>
constexpr vec2<T> operator!=(const vec2<T> a, const T b)
{
    return {static_cast<T>(a.x != b), static_cast<T>(a.y != b)};
}

template <class T>
constexpr vec2<T> operator<(const T a, const vec2<T> b)
{
    return {static_cast<T>(a < b.x), static_cast<T>(a < b.y)};
}

template <class T>
constexpr vec2<T> operator>(const T a, const vec2<T> b)
{
    return {static_cast<T>(a > b.x), static_cast<T>(a > b.y)};
}

template <class T>
constexpr vec2<T> operator<=(const T a, const vec2<T> b)
{
    return {static_cast<T>(a <= b.x), static_cast<T>(a <= b.y)};
}

template <class T>
constexpr vec2<T> operator>=(const T a, const vec2<T> b)
{
    return {static_cast<T>(a >= b.x), static_cast<T>(a >= b.y)};
}

template <class T>
constexpr vec2<T> operator==(const T a, const vec2<T> b)
{
    return {static_cast<T>(a == b.x), static_cast<T>(a == b.y)};
}

template <class T>
constexpr vec2<T> operator!=(const T a, const vec2<T> b)
{
    return {static_cast<T>(a != b.x), static_cast<T>(a != b.y)};
}

template <class T>
constexpr vec2<T> operator<(const vec2<T> a, const vec2<T> b)
{
    return {static_cast<T>(a.x < b.x), static_cast<T>(a.y < b.y)};
}

template <class T>
constexpr vec2<T> operator>(const vec2<T> a, const vec2<T> b)
{
    return {static_cast<T>(a.x > b.x), static_cast<T>(a.y > b.y)};
}

template <class T>
constexpr vec2<T> operator<=(const vec2<T> a, const vec2<T> b)
{
    return {static_cast<T>(a.x <= b.x), static_cast<T>(a.y <= b.y)};
}

template <class T>
constexpr vec2<T> operator>=(const vec2<T> a, const vec2<T> b)
{
    return {static_cast<T>(a.x >= b.x), static_cast<T>(a.y >= b.y)};
}
}
#endif // MYTEC_VECTORS_HPP
