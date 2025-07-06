#ifndef MYTEC_VECTORS_HPP
#define MYTEC_VECTORS_HPP

#include "SFML/System/Vector2.hpp"

#include <algorithm>
#include <cstdint>

namespace mytec
{
using vi2 = sf::Vector2<int32_t>;
using vf2 = sf::Vector2<float>;
using vu2 = sf::Vector2<uint32_t>;

template <class T>
constexpr sf::Vector2<T> operator+(const sf::Vector2<T> _a, const T _v)
{
    return _a + sf::Vector2{_v, _v};
}

template <class T>
constexpr sf::Vector2<T> operator-(const sf::Vector2<T> _a, const T _v)
{
    return {_a.x - _v, _a.y - _v};
}

template <class T>
constexpr sf::Vector2<T> operator-(const T _v, const sf::Vector2<T> _a)
{
    return sf::Vector2{_v, _v} - _a;
}

template <class T>
constexpr sf::Vector2<T> operator*(const sf::Vector2<T> _a, const sf::Vector2<T> _b)
{
    return {_a.x * _b.x, _a.y * _b.y};
}

template <class T>
constexpr sf::Vector2<T>& operator*=(sf::Vector2<T>& _a, const sf::Vector2<T> _b)
{
    return _a = _a * _b;
}

template <class T>
constexpr sf::Vector2<T> operator/(const sf::Vector2<T> _a, const sf::Vector2<T> _b)
{
    return {_a.x / _b.x, _a.y / _b.y};
}

template <class T>
constexpr sf::Vector2<T>& operator/=(sf::Vector2<T>& _a, const sf::Vector2<T> _b)
{
    return _a = _a / _b;
}

template <class T>
constexpr sf::Vector2<T> clamp(const sf::Vector2<T> _v, const sf::Vector2<T> _min, const sf::Vector2<T> _max)
{
    return {std::clamp(_v.x, _min.x, _max.x), std::clamp(_v.y, _min.y, _max.y)};
}

template <class T>
constexpr sf::Vector2<T> max(const sf::Vector2<T> _left, const sf::Vector2<T> _right)
{
    return {std::max(_left.x, _right.x), std::max(_left.y, _right.y)};
}

template <class T>
constexpr sf::Vector2<T> min(const sf::Vector2<T> _left, const sf::Vector2<T> _right)
{
    return {std::min(_left.x, _right.x), std::min(_left.y, _right.y)};
}

template <class T>
constexpr sf::Vector2<bool> operator<(const sf::Vector2<T> _a, const T _b)
{
    return {_a.x < _b, _a.y < _b};
}

template <class T>
constexpr sf::Vector2<bool> operator>(const sf::Vector2<T> _a, const T _b)
{
    return {_a.x > _b, _a.y > _b};
}

template <class T>
constexpr sf::Vector2<bool> operator<=(const sf::Vector2<T> _a, const T _b)
{
    return {_a.x <= _b, _a.y <= _b};
}

template <class T>
constexpr sf::Vector2<bool> operator>=(const sf::Vector2<T> _a, const T _b)
{
    return {_a.x >= _b, _a.y >= _b};
}

template <class T>
constexpr sf::Vector2<bool> operator==(const sf::Vector2<T> _a, const T _b)
{
    return {_a.x == _b, _a.y == _b};
}

template <class T>
constexpr sf::Vector2<bool> operator!=(const sf::Vector2<T> _a, const T _b)
{
    return {_a.x != _b, _a.y != _b};
}

template <class T>
constexpr sf::Vector2<bool> operator<(const T _a, const sf::Vector2<T> _b)
{
    return {_a < _b.x, _a < _b.y};
}

template <class T>
constexpr sf::Vector2<bool> operator>(const T _a, const sf::Vector2<T> _b)
{
    return {_a > _b.x, _a > _b.y};
}

template <class T>
constexpr sf::Vector2<bool> operator<=(const T _a, const sf::Vector2<T> _b)
{
    return {_a <= _b.x, _a <= _b.y};
}

template <class T>
constexpr sf::Vector2<bool> operator>=(const T _a, const sf::Vector2<T> _b)
{
    return {_a >= _b.x, _a >= _b.y};
}

template <class T>
constexpr sf::Vector2<bool> operator==(const T _a, const sf::Vector2<T> _b)
{
    return {_a == _b.x, _a == _b.y};
}

template <class T>
constexpr sf::Vector2<bool> operator!=(const T _a, const sf::Vector2<T> _b)
{
    return {_a != _b.x, _a != _b.y};
}

template <class T>
constexpr sf::Vector2<bool> operator<(const sf::Vector2<T> _a, const sf::Vector2<T> _b)
{
    return {_a.x < _b.x, _a.y < _b.y};
}

template <class T>
constexpr sf::Vector2<bool> operator>(const sf::Vector2<T> _a, const sf::Vector2<T> _b)
{
    return {_a.x > _b.x, _a.y > _b.y};
}

template <class T>
constexpr sf::Vector2<bool> operator<=(const sf::Vector2<T> _a, const sf::Vector2<T> _b)
{
    return {_a.x <= _b.x, _a.y <= _b.y};
}

template <class T>
constexpr sf::Vector2<bool> operator>=(const sf::Vector2<T> _a, const sf::Vector2<T> _b)
{
    return {_a.x >= _b.x, _a.y >= _b.y};
}
} // namespace mytec
#endif // MYTEC_VECTORS_HPP
