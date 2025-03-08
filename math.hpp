#pragma once

#include <cstddef>
#include <cstdint>
#include <tuple>
#include <utility>
#include <cmath>
#include <type_traits>
#include <numeric>

namespace gf::math{

namespace detail{
  inline static constexpr auto NotSpecialized = false;

  template<typename T, typename... Targs>
  using all_same = std::enable_if_t<std::conjunction_v<std::is_same<T, Targs>...>>;
} //namespace detail

template<typename T, std::size_t N, bool specialized = true>
struct vec{
  using array_type = T[N];

  array_type dims{};

  constexpr vec() noexcept = default;

  constexpr vec(T value) noexcept{
    for (int i = 0; i < N; ++i){
      dims[i] = value;
    }
  }

  constexpr vec(const array_type& array) noexcept{
    for (int i = 0; i < N; ++i){
      dims[i] = array[i];
    }
  }

  constexpr vec(const vec<T, N>& v) : vec(v.dims) {}

  template<typename... Targs, typename = detail::all_same<T, Targs...>>
  constexpr vec(Targs&&... args) noexcept 
  : dims{ std::forward<Targs>(args)... } {}

  constexpr auto len_squared() const noexcept{
    auto sum = 0.0;

    for (auto& e : dims){
      sum += e * e;
    }

    return sum;
  }

  template<typename Callable>
  constexpr auto map(Callable callable) const noexcept{
    using item_type = decltype(callable(std::declval<T&>()));

    auto result = vec<item_type, N>();

    for (int i = 0; i < N; ++i){
      result.dims[i] = callable(dims[i]);
    }

    return result;
  }

  constexpr auto len() const noexcept{
    return std::sqrt(len_squared());
  }

  constexpr auto normalized() const noexcept{
    return vec<T, N>((*this / len()));
  }

  constexpr auto& operator+=(const vec& other) noexcept{
    return (*this) = (*this) + other;
  }

  constexpr auto& operator+=(const T& x) noexcept{
    return (*this) = (*this) + x;
  }

  constexpr auto& operator-=(const vec& other) noexcept{
    return (*this) = (*this) - other;
  }

  constexpr auto& operator-=(const T& x) noexcept{
    return (*this) = (*this) - x;
  }

  constexpr auto& operator*=(const vec& other) noexcept{
    return (*this) = (*this) * other;
  }

  constexpr auto& operator*=(const T& x) noexcept{
    return (*this) = (*this) * x;
  }

  constexpr auto& operator/=(const vec& other) noexcept{
    return (*this) = (*this) / other;
  }

  constexpr auto& operator/=(const T& x) noexcept{
    return (*this) = (*this) / x;
  }
};

template<std::size_t I, typename T, std::size_t N>
auto get(const vec<T, N>& vec) noexcept{
  return vec.dims[I];
}

template<std::size_t I, typename T, std::size_t N>
auto& get(vec<T, N>& vec) noexcept{
  return vec.dims[I];
}

template<std::size_t I, typename T, std::size_t N>
auto&& get(vec<T, N>&& vec) noexcept{
  return std::move(vec.dims[I]);
}

template<std::size_t I, typename T, std::size_t N>
const auto&& get(const vec<T, N>&& vec) noexcept{
  return std::move(vec.dims[I]);
}

template<typename T>
struct vec<T, 2>{
  union{
    T dims[2]{};
    struct { T x, y; };
  };

  constexpr vec() noexcept = default;
  constexpr explicit vec(T value) : dims{ value, value } {}
  constexpr vec(T x, T y) : x(x), y(y) {}

  constexpr auto len_squared() const noexcept{
    return vec<T, 2, detail::NotSpecialized>(dims).len_squared();
  }

  constexpr auto len() const noexcept{
    return vec<T, 2, detail::NotSpecialized>(dims).len();
  }

  constexpr auto normalized() const noexcept{
    return vec<T, 2, detail::NotSpecialized>(dims).normalized();
  }

  constexpr auto& operator+=(const vec& other) noexcept{
    return (*this) = (*this) + other;
  }

  constexpr auto& operator+=(const T& x) noexcept{
    return (*this) = (*this) + x;
  }

  constexpr auto& operator-=(const vec& other) noexcept{
    return (*this) = (*this) - other;
  }

  constexpr auto& operator-=(const T& x) noexcept{
    return (*this) = (*this) - x;
  }

  constexpr auto& operator*=(const vec& other) noexcept{
    return (*this) = (*this) * other;
  }

  constexpr auto& operator*=(const T& x) noexcept{
    return (*this) = (*this) * x;
  }

  constexpr auto& operator/=(const vec& other) noexcept{
    return (*this) = (*this) / other;
  }

  constexpr auto& operator/=(const T& x) noexcept{
    return (*this) = (*this) / x;
  }
};

template<typename T>
struct vec<T, 3>{
  union{
    T dims[3]{};
    struct { T x, y, z; };
    struct { T r, g, b; };
  };

  constexpr vec() noexcept = default;
  constexpr explicit vec(T value) : dims{ value, value, value } {}
  constexpr vec(T x, T y, T z) : x(x), y(y), z(z) {}

  constexpr auto len_squared() const noexcept{
    return vec<T, 3, detail::NotSpecialized>(dims).len_squared();
  }

  constexpr auto len() const noexcept{
    return vec<T, 3, detail::NotSpecialized>(dims).len();
  }

  constexpr auto normalized() const noexcept{
    return vec<T, 3, detail::NotSpecialized>(dims).normalized();
  }

  constexpr auto& operator+=(const vec& other) noexcept{
    return (*this) = (*this) + other;
  }

  constexpr auto& operator+=(const T& x) noexcept{
    return (*this) = (*this) + x;
  }

  constexpr auto& operator-=(const vec& other) noexcept{
    return (*this) = (*this) - other;
  }

  constexpr auto& operator-=(const T& x) noexcept{
    return (*this) = (*this) - x;
  }

  constexpr auto& operator*=(const vec& other) noexcept{
    return (*this) = (*this) * other;
  }

  constexpr auto& operator*=(const T& x) noexcept{
    return (*this) = (*this) * x;
  }

  constexpr auto& operator/=(const vec& other) noexcept{
    return (*this) = (*this) / other;
  }

  constexpr auto& operator/=(const T& x) noexcept{
    return (*this) = (*this) / x;
  }
};

template<typename T>
struct vec<T, 4>{
  union{
    T dims[4]{};
    struct { T x, y, z, w; };
    struct { T r, g, b, a; };
  };

  constexpr vec() noexcept = default;
  constexpr explicit vec(T value) : dims{ value, value, value, value } {}
  constexpr vec(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

  template<typename Callable>
  constexpr auto map(Callable callable) const noexcept{
    return vec<T, 4, detail::NotSpecialized>(dims).map(callable);
  }

  constexpr auto len_squared() const noexcept{
    return vec<T, 4, detail::NotSpecialized>(dims).len_squared();
  }

  constexpr auto len() const noexcept{
    return vec<T, 4, detail::NotSpecialized>(dims).len();
  }

  constexpr auto normalized() const noexcept{
    return vec<T, 4, detail::NotSpecialized>(dims).normalized();
  }

  constexpr auto& operator+=(const vec& other) noexcept{
    return (*this) = (*this) + other;
  }

  constexpr auto& operator+=(const T& x) noexcept{
    return (*this) = (*this) + x;
  }

  constexpr auto& operator-=(const vec& other) noexcept{
    return (*this) = (*this) - other;
  }

  constexpr auto& operator-=(const T& x) noexcept{
    return (*this) = (*this) - x;
  }

  constexpr auto& operator*=(const vec& other) noexcept{
    return (*this) = (*this) * other;
  }

  constexpr auto& operator*=(const T& x) noexcept{
    return (*this) = (*this) * x;
  }

  constexpr auto& operator/=(const vec& other) noexcept{
    return (*this) = (*this) / other;
  }

  constexpr auto& operator/=(const T& x) noexcept{
    return (*this) = (*this) / x;
  }
};

template<typename T, std::size_t N, bool S>
inline constexpr auto zip(const vec<T, N, S>& v1, const vec<T, N, S>& v2){
  auto result = vec<std::pair<T, T>, N, true>();

  for (int i = 0; i < N; ++i){
    result.dims[i] = std::make_pair(v1.dims[i], v2.dims[i]);
  }

  return result;
}

template<typename T, std::size_t N, bool S>
inline constexpr auto operator-(const vec<T, N, S>& v) noexcept{
  return v.map([&](const auto& e) { return -e; });
}

template<typename T, std::size_t N, bool S>
inline constexpr auto operator==(const vec<T, N, S>& lhs, const vec<T, N, S>& rhs) noexcept{
  for (int i = 0; i < N; ++i){
    if (lhs.dims[i] != rhs.dims[i]) return false;
  }

  return true;
}

template<typename T, std::size_t N, bool S>
inline constexpr auto operator!=(const vec<T, N, S>& lhs, const vec<T, N, S>& rhs) noexcept{
  return !(lhs == rhs);
}

template<typename T, std::size_t N, bool S>
inline constexpr auto operator+(const vec<T, N, S>& lhs, const vec<T, N, S>& rhs) noexcept{
  return zip(lhs, rhs).map([&](const auto& p){
    return p.first + p.second;
  });
}

template<typename T, std::size_t N, bool S>
inline constexpr auto operator*(const vec<T, N, S>& lhs, const vec<T, N, S>& rhs) noexcept{
  return zip(lhs, rhs).map([&](const auto& p){
    return p.first * p.second;
  });
}

template<typename T, std::size_t N, bool S>
inline constexpr auto operator/(const vec<T, N, S>& lhs, const vec<T, N, S>& rhs) noexcept{
  return lhs * (1.0 / rhs);
}

template<typename T, std::size_t N, bool S>
inline constexpr auto operator*(const vec<T, N, S>& v, T x) noexcept{
  return v.map([&](const auto& e){ return e * x; });
}

template<typename T, std::size_t N, bool S>
inline constexpr auto operator-(const vec<T, N, S>& lhs, const vec<T, N, S>& rhs) noexcept{
  return lhs + (-rhs);
}

template<typename T, std::size_t N, bool S>
inline constexpr auto operator+(const vec<T, N, S>& v, T x){
  return v + vec<T, N>(x);
}

template<typename T, std::size_t N, bool S>
inline constexpr auto operator+(T x, const vec<T, N, S>& v){
  return v + x;
}

template<typename T, std::size_t N, bool S>
inline constexpr auto operator-(T x, const vec<T, N, S>& v){
  return -v + x;
}

template<typename T, std::size_t N, bool S>
inline constexpr auto operator-(const vec<T, N, S>& v, T x){
  return v + (-x);
}

template<typename T, std::size_t N, bool S>
inline constexpr auto operator*(T x, const vec<T, N, S>& v) noexcept{
  return v * x;
}

template<typename T, std::size_t N, bool S>
inline constexpr auto operator/(const vec<T, N, S>& v, T x) noexcept{
  return v * (1.0 / x);
}

template<typename T, std::size_t N, bool S>
inline constexpr auto operator/(T x, const vec<T, N, S>& v) noexcept{
  return v.map([&](const auto& e){
    return x / e; 
  });
}

template<typename T, std::size_t N, bool S>
inline constexpr auto dot_product(const vec<T, N, S>& v1, const vec<T, N, S>& v2){
  const auto v = v1 * v2;

  return std::accumulate(v.dims, v.dims + N, 0.0, std::plus{});
}

template<typename T, std::size_t N, bool S>
inline constexpr auto max(const vec<T, N, S>& v1, const vec<T, N, S>& v2){
  return zip(v1, v2).map([&](const auto& p){
    return std::max(p.first, p.second);
  });
}

template<typename T, std::size_t N, bool S>
inline constexpr auto min(const vec<T, N, S>& v1, const vec<T, N, S>& v2){
  return zip(v1, v2).map([&](const auto& p){
    return std::min(p.first, p.second);
  });
}

template<typename T, std::size_t N, bool S>
inline constexpr auto clamp(
    const vec<T, N, S>& v, 
    const vec<T, N, S>& v_min,
    const vec<T, N, S>& v_max
){
  return max(v_min, min(v, v_max));
}

using vec2 = vec<double, 2>;
using fvec2 = vec<float, 2>;
using ivec2 = vec<std::int32_t, 2>;

using vec3 = vec<double, 3>;
using fvec3 = vec<float, 3>;
using ivec3 = vec<std::int32_t, 3>;

using vec4 = vec<double, 4>;
using fvec4 = vec<float, 4>;
using ivec4 = vec<std::int32_t, 4>;

} //namespace gf::math

//For structured binding to work:
namespace std{

template<typename T, size_t N, bool S>
struct tuple_size<gf::math::vec<T, N, S>> : integral_constant<size_t, N>{};

template<size_t I, typename T, size_t N, bool S>
struct tuple_element<I, gf::math::vec<T, N, S>>{
  using type = T;
  static_assert(I < N, "Structured binding's out of bounds vec");
};

} //namespace std

#ifdef GEFEC_MATH_DEBUG

#include <iostream>

template<typename T, std::size_t N, bool S>
auto operator<<(std::ostream& out, const gf::math::vec<T, N, S>& vec)
-> std::ostream&{
  out << '[';

  for (int i = 0; i < N - 1; ++i){
    out << vec.dims[i] << ',';
  }

  return out << vec.dims[N - 1] << ']';
}

#endif
