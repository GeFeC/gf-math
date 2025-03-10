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

  template<typename T>
  using arithmetic = std::enable_if_t<std::is_arithmetic_v<T>>;
} //namespace detail

template<typename T>
struct range_base{
  T min;
  T max;

  struct iterator{
    T value;
    constexpr auto operator==(const iterator& other) const noexcept{
      return value == other.value;
    }

    constexpr auto operator!=(const iterator& other) const noexcept{
      return !(*this == other);
    }

    constexpr auto operator++() noexcept{
      ++value;
      return value;
    }

    constexpr auto operator*() const noexcept{
      return value;
    }
  };

  auto begin() const noexcept{
    return iterator{ min };
  }

  auto end() const noexcept{
    return iterator{ max };
  }
};

template<typename E>
struct range_base<std::pair<E, E>>{
  using T = std::pair<E, E>;

  T min, max;

  struct iterator{
    T value;
    T min, max;

    constexpr auto operator==(const iterator& other) const noexcept{
      return value.second == other.value.second;
    }

    constexpr auto operator!=(const iterator& other) const noexcept{
      return !(*this == other);
    }

    constexpr auto operator++() noexcept{
      ++value.first;

      if (value.first == max.first){
        ++value.second;
        value.first = min.first;
      }

      return value;
    }

    constexpr auto operator*() const noexcept{
      return value;
    }
  };

  auto begin() const noexcept{
    return iterator{ min, min, max };
  }

  auto end() const noexcept{
    return iterator{ max, min, max };
  }
};


inline constexpr auto range(std::size_t min, std::size_t max) noexcept{
  return range_base<std::size_t>{ min, max }; 
}

inline constexpr auto range(const std::size_t& max) noexcept{
  return range(0, max);
}

inline constexpr auto range(
    const std::pair<std::size_t, std::size_t>& min, 
    const std::pair<std::size_t, std::size_t>& max
) noexcept{
  return range_base<std::pair<std::size_t, std::size_t>>{ min, max }; 
}

inline constexpr auto range(const std::pair<std::size_t, std::size_t>& max) noexcept{
  return range({ 0, 0 }, max);
}

template<typename T, std::size_t W, std::size_t H, bool S>
struct mat;

template<typename T, std::size_t N, bool S = true>
struct vec{
  using array_type = T[N];

  array_type dims{};

  constexpr vec() noexcept = default;

  constexpr vec(T value) noexcept{
    for (auto& d : dims){
      d = value;
    }
  }

  constexpr vec(const array_type& array) noexcept{
    for (auto i : range(N)){
      dims[i] = array[i];
    }
  }

  constexpr vec(const vec<T, N>& v) : vec(v.dims) {}

  template<typename... Targs, typename = detail::all_same<T, Targs...>>
  constexpr vec(Targs&&... args) noexcept 
  : dims{ std::forward<Targs>(args)... } {}

  template<typename Callable>
  constexpr auto map(Callable callable) const noexcept{
    using item_type = decltype(callable(std::declval<T&>()));

    auto result = vec<item_type, N>();

    for (auto i : range(N)){
      result.dims[i] = callable(dims[i]);
    }

    return result;
  }

  template<typename Callable>
  constexpr auto every(Callable callable) const noexcept{
    for (auto i : range(N)){
      if (!callable(dims[i])) return false;
    }

    return true;
  }

  constexpr auto len_squared() const noexcept{
    auto sum = 0.0;

    for (auto& e : dims){
      sum += e * e;
    }

    return sum;
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

  template<bool matS>
  constexpr auto operator*=(const mat<T, N, N, matS>& mat) noexcept -> vec&;

  template<bool matS>
  constexpr auto operator/=(const mat<T, N, N, matS>& mat) noexcept -> vec&;
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
    struct { T left, right; };
    struct { T top, bottom; };
  };

  constexpr vec() noexcept = default;
  constexpr explicit vec(T value) : dims{ value, value } {}
  constexpr vec(T x, T y) : x(x), y(y) {}

  template<typename Callable>
  constexpr auto every(Callable callable) const noexcept{
    return vec<T, 2, detail::NotSpecialized>(dims).every(callable);
  }

  template<typename Callable>
  constexpr auto map(Callable callable) const noexcept{
    return vec<T, 2, detail::NotSpecialized>(dims).map(callable);
  }

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

  template<bool matS>
  constexpr auto operator*=(const mat<T, 2, 2, matS>& mat) noexcept -> vec&;

  template<bool matS>
  constexpr auto operator/=(const mat<T, 2, 2, matS>& mat) noexcept -> vec&;
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

  template<typename Callable>
  constexpr auto every(Callable callable) const noexcept{
    return vec<T, 3, detail::NotSpecialized>(dims).every(callable);
  }

  template<typename Callable>
  constexpr auto map(Callable callable) const noexcept{
    return vec<T, 3, detail::NotSpecialized>(dims).map(callable);
  }

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

  template<bool matS>
  constexpr auto operator*=(const mat<T, 3, 3, matS>& mat) noexcept -> vec&;

  template<bool matS>
  constexpr auto operator/=(const mat<T, 3, 3, matS>& mat) noexcept -> vec&;
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

  template<typename Callable>
  constexpr auto every(Callable callable) const noexcept{
    return vec<T, 4, detail::NotSpecialized>(dims).every(callable);
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

  template<bool matS>
  constexpr auto operator*=(const mat<T, 4, 4, matS>& mat) noexcept -> vec&;

  template<bool matS>
  constexpr auto operator/=(const mat<T, 4, 4, matS>& mat) noexcept -> vec&;

};

template<typename T, std::size_t N, bool S>
inline constexpr auto zip(const vec<T, N, S>& v1, const vec<T, N, S>& v2) noexcept{
  auto result = vec<std::pair<T, T>, N, true>();

  for (auto i : range(N)){
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
  return zip(lhs, rhs).every([](const auto& p){
    return p.first == p.second;
  });
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
inline constexpr auto operator+(const vec<T, N, S>& v, T x) noexcept{
  return v + vec<T, N>(x);
}

template<typename T, std::size_t N, bool S>
inline constexpr auto operator+(T x, const vec<T, N, S>& v) noexcept{
  return v + x;
}

template<typename T, std::size_t N, bool S>
inline constexpr auto operator-(T x, const vec<T, N, S>& v) noexcept{
  return -v + x;
}

template<typename T, std::size_t N, bool S>
inline constexpr auto operator-(const vec<T, N, S>& v, T x) noexcept{
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
inline constexpr auto dot(const vec<T, N, S>& v1, const vec<T, N, S>& v2) noexcept{
  const auto v = v1 * v2;

  return std::accumulate(v.dims, v.dims + N, 0.0, std::plus{});
}

template<typename T, bool S>
inline constexpr auto cross(const vec<T, 3, S>& v1, const vec<T, 3, S>& v2) noexcept{
  const auto [a1, a2, a3] = v1;
  const auto [b1, b2, b3] = v2;

  return vec<T, 3, S>(
    a2 * b3 - a3 * b2,
    a3 * b1 - a1 * b3,
    a1 * b2 - a2 * b1
  );
}

template<typename T, std::size_t N, bool S>
inline constexpr auto max(const vec<T, N, S>& v1, const vec<T, N, S>& v2) noexcept{
  return zip(v1, v2).map([&](const auto& p){
    return std::max(p.first, p.second);
  });
}

template<typename T, std::size_t N, bool S>
inline constexpr auto min(const vec<T, N, S>& v1, const vec<T, N, S>& v2) noexcept{
  return zip(v1, v2).map([&](const auto& p){
    return std::min(p.first, p.second);
  });
}

template<typename T, std::size_t N, bool S>
inline constexpr auto clamp(
    const vec<T, N, S>& v, 
    const vec<T, N, S>& v_min,
    const vec<T, N, S>& v_max
) noexcept{
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

template<typename T, std::size_t W, std::size_t H, bool S = true>
struct mat{
  using array_type = T[W][H];
  array_type data;

  constexpr mat() noexcept : data{} {}

  template<typename... Targs, typename = detail::all_same<T, Targs...>>
  constexpr mat(Targs&&... args) noexcept{
    T array[] = { args... };

    for (const auto& [x, y] : range({ W, H })){
      data[x][y] = array[y * W + x];
    }
  }

  constexpr auto& operator=(const mat<T, W, H, !S>& m){
    for (const auto& [x, y] : range({ W, H })){
      data[x][y] = m.data[x][y];
    }

    return *this;
  }

  constexpr const auto& operator[](std::size_t x) const noexcept{
    return data[x];
  }

  constexpr auto& operator[](std::size_t x) noexcept{
    return data[x];
  }

  template<typename Callable>
  constexpr auto map(Callable callable) const noexcept{
    using item_type = decltype(callable(std::declval<T&>()));

    auto result = mat<item_type, W, H>();

    for (const auto& [x, y] : range({ W, H })){
      result[x][y] = callable(data[x][y]);
    }

    return result;
  }

  template<typename Callable>
  constexpr auto every(Callable callable) const noexcept{
    for (const auto& [x, y] : range({ W, H })){
      if (!callable(data[x][y])) return false;
    }

    return true;
  }

  constexpr auto t() const noexcept{
    auto result = mat<T, H, W>();

    for (const auto& [x, y] : range({ W, H })){
      result[y][x] = data[x][y];
    }
    
    return result;
  }

  constexpr auto row(std::size_t n) const noexcept{
    auto result = vec<T, W>();

    for (auto i : range(W)){
      result.dims[i] = data[i][n];
    }

    return result;
  }

  constexpr auto col(std::size_t n) const noexcept{
    auto result = vec<T, W>();

    for (auto i : range(W)){
      result.dims[i] = data[n][i];
    }

    return result;
  }

  constexpr auto set_row(std::size_t n, const vec<T, W>& v) noexcept{
    for (auto i : range(W)){
      data[i][n] = v.dims[i];
    }
  }

  constexpr auto set_col(std::size_t n, const vec<T, H>& v) noexcept{
    for (auto i : range(H)){
      data[n][i] = v.dims[i];
    }
  }

  constexpr auto swap_rows(std::size_t r1, std::size_t r2) noexcept{
    const auto temp = row(r1);
    set_row(r1, row(r2));
    set_row(r2, temp);
  }

  constexpr auto is_any_row_zero() const noexcept{
    for (auto i : range(W)){
      if (row(i) != vec<T, W>(0.0)) return false;
    }

    return true;
  }; 

  constexpr auto is_any_column_zero() const noexcept{
    for (auto i : range(W)){
      if (col(i) != vec<T, W>(0.0)) return false;
    }

    return true;
  }; 

  constexpr auto& operator+=(const mat& other) noexcept{
    return (*this) = (*this) + other;
  }

  constexpr auto& operator+=(const T& x) noexcept{
    return (*this) = (*this) + x;
  }

  constexpr auto& operator-=(const mat& other) noexcept{
    return (*this) = (*this) - other;
  }

  constexpr auto& operator-=(const T& x) noexcept{
    return (*this) = (*this) - x;
  }

  constexpr auto& operator*=(const vec<T, W>& other) noexcept{
    return (*this) = (*this) * other;
  }

  constexpr auto& operator*=(const T& x) noexcept{
    return (*this) = (*this) * x;
  }

  constexpr auto& operator/=(const vec<T, W>& other) noexcept{
    return (*this) = (*this) / other;
  }

  constexpr auto& operator/=(const T& x) noexcept{
    return (*this) = (*this) / x;
  }
};

template<typename T, std::size_t N>
struct mat<T, N, N> : mat<T, N, N, detail::NotSpecialized>{
  constexpr mat() noexcept : mat<T, N, N, detail::NotSpecialized>() {}

  template<typename... Targs, typename = detail::all_same<T, Targs...>>
  constexpr mat(Targs&&... args) noexcept 
  : mat<T, N, N, detail::NotSpecialized>(std::forward<Targs>(args)...){}

  constexpr mat(T value) noexcept : mat() {
    for (auto i : range(N)){
      this->data[i][i] = value;
    }
  }

  constexpr auto diagonal_product() const noexcept{
    auto result = 1.0;
    for (auto i : range(N)){
      result *= this->data[i][i];
    }

    return result;
  }

  constexpr auto is_diagonal_zero() const noexcept{
    for (auto i : range(N)){
      if (this->data[i][i] != 0.0) return false;
    }

    return true;
  };

  constexpr auto is_upper_triangular() const noexcept{
    for (auto x : range(N - 1)){
      for (auto y : range(x + 1, N)){
        if (this->data[x][y] != 0.0) return false;
      }
    }

    return true;
  }

  constexpr auto is_lower_triangular() const noexcept{
    for (auto x : range(1, N)){
      for (auto y : range(x)){
        if (this->data[x][y] != 0.0) return false;
      }
    }

    return true;
  }

  constexpr auto is_triangular() const noexcept{
    return is_upper_triangular() || is_lower_triangular();
  }

  constexpr auto is_diagonal() const noexcept{
    return is_upper_triangular() && is_lower_triangular();
  }

  constexpr auto det() const noexcept -> T{
    auto mat = (*this);
    auto sign = 1;

    for (auto x : range(N - 1)){
      if (mat.is_any_row_zero() || mat.is_any_column_zero() || mat.is_diagonal_zero()) return 0.0;

      if (mat.is_triangular()){
        return sign * mat.diagonal_product();
      }

      if (mat[x][x] == 0.0){
        for (auto y : range(N)){
          if (mat[x][y] != 0.0 && mat[y][x] != 0.0){
            sign = -sign;
            mat.swap_rows(x, y);
            break;
          }
        }
      }

      for (auto y : range(x + 1, N)){
        mat.set_row(y, mat.row(y) - mat.row(x) * mat[x][y] / mat[x][x]);
      }
    }
    return sign * mat.diagonal_product();
  }
};

template<typename T, std::size_t W, std::size_t H, bool S1, bool S2>
inline constexpr auto zip(const mat<T, W, H, S1>& m1, const mat<T, W, H, S2>& m2) noexcept{
  auto result = mat<std::pair<T, T>, W, H>();

  for (const auto& [x, y] : range({ W, H })){
    result[x][y] = std::make_pair(m1[x][y], m2[x][y]);
  }

  return result;
}

template<typename T, std::size_t N, bool S>
inline constexpr auto to_mat(const vec<T, N, S>& vec) noexcept{
  auto result = mat<T, N, 1>();

  for (auto i : range(N)){
    result[i][0] = vec.dims[i];
  }

  return result;
}

template<typename T, bool S>
inline constexpr auto to_vec(const mat<T, 1, 1, S>& mat) noexcept{
  return vec<T, 1>(mat[0][0]);
}

template<typename T, std::size_t N, bool S>
inline constexpr auto to_vec(const mat<T, N, 1, S>& mat) noexcept{
  auto result = vec<T, N>();

  for (auto i : range(N)){
    result.dims[i] = mat[i][0];
  }

  return result;
}

template<typename T, std::size_t N, bool S>
inline constexpr auto to_vec(const mat<T, 1, N, S>& mat) noexcept{
  return to_vec(mat.t());
}

template<typename T, std::size_t W, std::size_t H, bool S1, bool S2>
inline constexpr auto operator==(const mat<T, W, H, S1>& m1, const mat<T, W, H, S2>& m2) noexcept{
  return zip(m1, m2).every([](const auto& p){
    return p.first == p.second;
  });
}

template<typename T, std::size_t W, std::size_t H, bool S1, bool S2>
inline constexpr auto operator!=(const mat<T, W, H, S1>& m1, const mat<T, W, H, S2>& m2) noexcept{
  return !(m1 == m2);
}

template<typename T, std::size_t W, std::size_t H, bool S1, bool S2>
inline constexpr auto operator+(const mat<T, W, H, S1>& m1, const mat<T, W, H, S2>& m2) noexcept{
  return zip(m1, m2).map([](const auto& p){
    return p.first + p.second;
  });
}

template<typename T, std::size_t W, std::size_t H, bool S>
inline constexpr auto operator+(const mat<T, W, H, S>& mat, T value) noexcept{
  return mat.map([&](const auto& e){
    return e + value;
  });
}

template<typename T, std::size_t W, std::size_t H, bool S>
inline constexpr auto operator+(T value, const mat<T, W, H, S>& mat) noexcept{
  return mat + value;
}

template<typename T, std::size_t W, std::size_t H, bool S>
inline constexpr auto operator-(const mat<T, W, H, S>& mat) noexcept{
  return mat.map([](const auto& e){
    return -e;
  });
}

template<typename T, std::size_t W, std::size_t H, bool S1, bool S2>
inline constexpr auto operator-(const mat<T, W, H, S1>& m1, const mat<T, W, H, S2>& m2) noexcept{
  return m1 + (-m2);
}

template<typename T, std::size_t W, std::size_t H, bool S>
inline constexpr auto operator-(const mat<T, W, H, S>& mat, T value) noexcept{
  return mat + (-value);
}

template<typename T, std::size_t W, std::size_t H, bool S>
inline constexpr auto operator-(T value, const mat<T, W, H, S>& mat) noexcept{
  return value + (-mat);
}

template<typename T, std::size_t W, std::size_t H, bool S>
inline constexpr auto operator*(const mat<T, W, H, S>& mat, T value) noexcept{
  return mat.map([&](const auto& e){
    return e * value;
  });
}

template<typename T, std::size_t W, std::size_t H, bool S>
inline constexpr auto operator*(T value, const mat<T, W, H, S>& mat) noexcept{
  return mat * value;
}

template<typename T, std::size_t W, std::size_t H1, std::size_t W2, bool S1, bool S2>
inline constexpr auto operator*(const mat<T, W, H1, S1>& m1, const mat<T, W2, W, S2>& m2) noexcept{
  auto result = mat<T, W2, H1>();

  for (const auto& [x, y] : range({ W2, H1 })){
    for (auto i : range(W)){
      result[x][y] += m1[i][y] * m2[x][i];
    }
  }

  return result;
}

template<typename T, std::size_t W, std::size_t H, bool S1, bool S2>
inline constexpr auto operator*(const vec<T, H, S2>& vec, const mat<T, W, H, S1>& mat) noexcept{
  const auto vec_mat = to_mat(vec);

  return to_vec(vec_mat * mat);
}

template<typename T, std::size_t W, std::size_t H, bool S1, bool S2>
inline constexpr auto operator*(const mat<T, W, H, S1>& mat, const vec<T, W, S2>& vec) noexcept{
  const auto vec_mat = to_mat(vec);

  return to_vec(vec_mat * mat);
}

template<typename T, std::size_t W, std::size_t H, bool S>
inline constexpr auto operator/(const mat<T, W, H, S>& mat, T value) noexcept{
  return mat * (1.0 / value);
}

template<typename T, std::size_t W, std::size_t H, bool S>
inline constexpr auto operator/(T value, const mat<T, W, H, S>& mat) noexcept{
  return mat.map([&](const auto& e){
    return value / e;
  });
}

template<typename T, std::size_t W, std::size_t H, std::size_t W2, bool S1, bool S2>
inline constexpr auto operator/(const mat<T, W, H, S1>& m1, const mat<T, W2, W, S2>& m2) noexcept{
  return m1 * (1.0 / m2);
}

template<typename T, std::size_t W, std::size_t H, bool S1, bool S2>
inline constexpr auto operator/(const vec<T, H, S2>& vec, const mat<T, W, H, S1>& mat) noexcept{
  return vec * (1.0 / mat);
}

template<typename T, std::size_t W, std::size_t H, bool S, bool S2>
inline constexpr auto operator/(const mat<T, W, H, S>& mat, const vec<T, W, S2>& vec) noexcept{
  return mat * (1.0 / vec);
}

template<typename T, std::size_t N, bool S>
template<bool S2>
constexpr auto vec<T, N, S>::operator*=(const mat<T, N, N, S2>& mat) noexcept -> vec&{
  return (*this) = (*this) * mat;
}

template<typename T, std::size_t N, bool S>
template<bool S2>
constexpr auto vec<T, N, S>::operator/=(const mat<T, N, N, S2>& mat) noexcept -> vec&{
  return (*this) = (*this) * mat;
}

template<typename T>
template<bool S>
constexpr auto vec<T, 2>::operator*=(const mat<T, 2, 2, S>& mat) noexcept -> vec&{
  return (*this) = (*this) * mat;
}

template<typename T>
template<bool S>
constexpr auto vec<T, 2>::operator/=(const mat<T, 2, 2, S>& mat) noexcept -> vec&{
  return (*this) = (*this) * mat;
}

template<typename T>
template<bool S>
constexpr auto vec<T, 3>::operator*=(const mat<T, 3, 3, S>& mat) noexcept -> vec&{
  return (*this) = (*this) * mat;
}

template<typename T>
template<bool S>
constexpr auto vec<T, 3>::operator/=(const mat<T, 3, 3, S>& mat) noexcept -> vec&{
  return (*this) = (*this) * mat;
}

template<typename T>
template<bool S>
constexpr auto vec<T, 4>::operator*=(const mat<T, 4, 4, S>& mat) noexcept -> vec&{
  return (*this) = (*this) * mat;
}

template<typename T>
template<bool S>
constexpr auto vec<T, 4>::operator/=(const mat<T, 4, 4, S>& mat) noexcept -> vec&{
  return (*this) = (*this) * mat;
}

using mat2 = mat<double, 2, 2>;
using imat2 = mat<std::int32_t, 2, 2>;
using mat3 = mat<double, 3, 3>;
using imat3 = mat<std::int32_t, 3, 3>;
using mat4 = mat<double, 4, 4>;
using imat4 = mat<std::int32_t, 4, 4>;

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
  out << "[ ";

  for (auto i : gf::math::range(N)){
    out << vec.dims[i] << ' ';
  }

  return out << ']';
}

template<typename T, std::size_t W, std::size_t H>
auto operator<<(std::ostream& out, const gf::math::mat<T, W, H>& mat)
-> std::ostream&{
  for (auto y : gf::math::range(H)){
    for (auto x : gf::math::range(W)){
      out << mat[x][y] << ' ';
    }
    out << '\n';
  }

  return out;
}


#endif
