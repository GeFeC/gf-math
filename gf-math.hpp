#pragma once

#ifdef _MSVC_VER
#define GF_CPP_VERSION _MSVC_VER
#else
#define GF_CPP_VERSION __cplusplus
#endif

#if GF_CPP_VERSION < 201402
#error "gf-math requires at least c++14 to work!"
#else

#ifndef GF_MATH_DEBUG
#define GF_MATH_DEBUG 1
#endif

#if GF_MATH_DEBUG == 1
#include <cassert>
#define GF_ASSERT(X) assert(X)
#else
#define GF_ASSERT(X)
#endif

#include <cstdint>
#include <limits>
#include <cmath>
#include <string>

namespace gf{

using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using if8 = std::int_fast8_t;
using if16 = std::int_fast16_t;
using if32 = std::int_fast32_t;
using if64 = std::int_fast64_t;

using uf8 = std::uint_fast8_t;
using uf16 = std::uint_fast16_t;
using uf32 = std::uint_fast32_t;
using uf64 = std::uint_fast64_t;

using f32 = float;
using f64 = double;

template<typename T>
inline constexpr auto max_limit = std::numeric_limits<T>::max();

template<typename T>
inline constexpr auto min_limit = std::numeric_limits<T>::min();

template<std::size_t N, typename T>
struct vec{};

template<typename T>
struct vec<2, T>{
  using this_t = vec<2, T>;
  using value_type = T;

  T x, y;

  explicit constexpr vec(T x, T y) : x(x), y(y) {}
  explicit constexpr vec(T value) : vec(value, value) {}
  explicit constexpr vec() = default;

  constexpr auto& r(){ return x; }
  constexpr auto r() const { return x; }
  constexpr auto& left(){ return x; }
  constexpr auto left() const { return x; }
  constexpr auto& width(){ return x; }
  constexpr auto width() const { return x; }

  constexpr auto& g(){ return y; }
  constexpr auto g() const { return y; }
  constexpr auto& right(){ return y; }
  constexpr auto right() const { return y; }
  constexpr auto& height(){ return y; }
  constexpr auto height() const { return y; }

  constexpr auto operator==(const this_t& other) const{
    return x == other.x && y == other.y; 
  }
  constexpr auto operator!=(const this_t& other) const{
    return !(*this == other); 
  }

  constexpr auto operator-() const{
    return this_t(-x, -y);
  }
  constexpr auto operator+(const this_t& other) const{
    return this_t(x + other.x, y + other.y);
  }
  constexpr auto operator-(const this_t& other) const{
    return this_t(x - other.x, y - other.y);
  }
  constexpr auto operator*(T value) const{
    return this_t(x * value, y * value);
  }
  constexpr auto operator/(T value) const{
    return this_t(x / value, y / value);
  }

  constexpr auto& operator+=(const this_t& other){
    x += other.x;
    y += other.y;

    return *this;
  }
  constexpr auto& operator-=(const this_t& other){
    x -= other.x;
    y -= other.y;

    return *this;
  }
  constexpr auto& operator*=(T value){
    x *= value;
    y *= value;

    return *this;
  }
  constexpr auto& operator/=(T value){
    x /= value;
    y /= value;

    return *this;
  }
};

using vec2 = vec<2, f32>;
using ivec2 = vec<2, i32>;
using uvec2 = vec<2, u32>;

template<typename T>
struct vec<3, T>{
  using this_t = vec<3, T>;
  using value_type = T;

  T x, y, z;

  explicit constexpr vec(T x, T y, T z = 0) : x(x), y(y), z(z) {}
  explicit constexpr vec(T value) : vec(value, value, value) {}
  explicit constexpr vec() = default;

  constexpr auto& r(){ return x; }
  constexpr auto r() const { return x; }
  constexpr auto& left(){ return x; }
  constexpr auto left() const { return x; }
  constexpr auto& width(){ return x; }
  constexpr auto width() const { return x; }

  constexpr auto& g(){ return y; }
  constexpr auto g() const { return y; }
  constexpr auto& right(){ return y; }
  constexpr auto right() const { return y; }
  constexpr auto& height(){ return y; }
  constexpr auto height() const { return y; }

  constexpr auto& b(){ return z; }
  constexpr auto b() const { return z; }
  constexpr auto& top(){ return z; }
  constexpr auto top() const { return z; }

  constexpr auto operator==(const this_t& other) const{
    return x == other.x && y == other.y && z == other.z; 
  }
  constexpr auto operator!=(const this_t& other) const{
    return !(*this == other); 
  }

  constexpr auto operator-() const{
    return this_t(-x, -y);
  }

  constexpr auto operator+(const this_t& other) const{
    return this_t(x + other.x, y + other.y, z + other.z);
  }
  constexpr auto operator-(const this_t& other) const{
    return this_t(x - other.x, y - other.y, z - other.z);
  }
  constexpr auto operator*(T value) const{
    return this_t(x * value, y * value, z * value);
  }
  constexpr auto operator/(T value) const{
    return this_t(x / value, y / value, z / value);
  }

  constexpr auto& operator+=(const this_t& other){
    x += other.x;
    y += other.y;
    z += other.z;

    return *this;
  }
  constexpr auto& operator-=(const this_t& other){
    x -= other.x;
    y -= other.y;
    z -= other.z;

    return *this;
  }
  constexpr auto& operator*=(T value){
    x *= value;
    y *= value;
    z *= value;

    return *this;
  }
  constexpr auto& operator/=(T value){
    x /= value;
    y /= value;
    z /= value;

    return *this;
  }
};

using vec3 = vec<3, f32>;
using ivec3 = vec<3, i32>;
using uvec3 = vec<3, u32>;

template<typename T>
struct vec<4, T>{
  using this_t = vec<4, T>;
  using value_type = T;

  T x, y, z, w;

  explicit constexpr vec(T x, T y, T z = 0, T w = 0) : x(x), y(y), z(z), w(w) {}
  explicit constexpr vec(T value) : vec(value, value, value, value) {}
  explicit constexpr vec() = default;

  constexpr auto& r(){ return x; }
  constexpr auto r() const { return x; }
  constexpr auto& left(){ return x; }
  constexpr auto left() const { return x; }
  constexpr auto& width(){ return x; }
  constexpr auto width() const { return x; }

  constexpr auto& g(){ return y; }
  constexpr auto g() const { return y; }
  constexpr auto& right(){ return y; }
  constexpr auto right() const { return y; }
  constexpr auto& height(){ return y; }
  constexpr auto height() const { return y; }
  
  constexpr auto& b(){ return z; }
  constexpr auto b() const { return z; }
  constexpr auto& top(){ return z; }
  constexpr auto top() const { return z; }

  constexpr auto& a(){ return w; }
  constexpr auto a() const { return w; }
  constexpr auto& bottom(){ return w; }
  constexpr auto bottom() const { return w; }

  constexpr auto operator-() const{
    return this_t(-x, -y, -z, -w);
  }

  constexpr auto operator==(const this_t& other) const{
    return x == other.x && y == other.y && z == other.z && w == other.w; 
  }
  constexpr auto operator!=(const this_t& other) const{
    return !(*this == other); 
  }

  constexpr auto operator+(const this_t& other) const{
    return this_t(x + other.x, y + other.y, z + other.z, w + other.w);
  }
  constexpr auto operator-(const this_t& other) const{
    return this_t(x - other.x, y - other.y, z - other.z, w - other.w);
  }
  constexpr auto operator*(T value) const{
    return this_t(x * value, y * value, z * value, w * value);
  }
  constexpr auto operator/(T value) const{
    return this_t(x / value, y / value, z / value, w / value);
  }

  constexpr auto& operator+=(const this_t& other){
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;

    return *this;
  }
  constexpr auto& operator-=(const this_t& other){
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;

    return *this;
  }
  constexpr auto& operator*=(T value){
    x *= value;
    y *= value;
    z *= value;
    w *= value;

    return *this;
  }
  constexpr auto& operator/=(T value){
    x /= value;
    y /= value;
    z /= value;
    w /= value;

    return *this;
  }
};

using vec4 = vec<4, f32>;
using ivec4 = vec<4, i32>;
using uvec4 = vec<4, u32>;

} //namespace gf

#endif