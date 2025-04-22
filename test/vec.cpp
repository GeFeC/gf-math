#define GEFEC_MATH_DEBUG
#include "../math.hpp"
#include "test.hpp"
#include <iomanip>

auto main() -> int{
  std::cerr << std::setprecision(100);

  namespace m = gf::math;

  static_assert(sizeof(m::vec<double, 10>) == sizeof(double) * 10);

  test("vec: all operators implemented", []{
    auto vec = m::vec4(1.f, 2.f, 3.f, 4.f);
    auto mat = m::mat4(1.f);
    auto x = 1.f;

    vec = vec;

    vec == vec;
    vec != vec;

    vec + x;
    x + vec;
    vec + vec;

    -vec;
    vec - x;
    x - vec;
    vec - vec;

    vec * x;
    x * vec;
    vec * vec;

    vec / x;
    x / vec;
    vec / vec;

    vec += x;
    vec += vec;

    vec -= x;
    vec -= vec;

    vec *= x;
    vec *= vec;

    vec /= x;
    vec /= vec;

    vec *= mat;
    vec /= mat;


    return true;
  });

  test("vec default init", []{
    return m::vec4() == m::vec4(0.f) && m::vec4() == m::vec4(0.f, 0.f, 0.f, 0.f);
  });

  test("vec single param ctor", []{
    auto vec = m::vec4(1234.f);
    return 
      vec.x == 1234.0 &&
      vec.y == vec.x &&
      vec.z == vec.x &&
      vec.w == vec.x;
  });

  auto vec = m::vec4(1.0, 2.0, 3.0, 4.0);

  test("vec assignment", [&]{
    return 
      vec.x == 1.0 &&
      vec.y == 2.0 &&
      vec.z == 3.0 &&
      vec.w == 4.0;
  });

  test("vec: union implemented properly", [&]{
    return
      vec.x == vec.r &&
      vec.y == vec.g &&
      vec.z == vec.b &&
      vec.w == vec.a;
  });

  test("vec: struct binding works", [&]{
    auto& [x, y, z, w] = vec;

    return 
      &vec.x == &x && 
      &vec.y == &y && 
      &vec.z == &z && 
      &vec.w == &w;
  });

  vec = m::vec4(0.5, -0.5, 0.0, 0.1);
  auto vec_copy = vec;
  auto vec2 = m::vec4(2.0, 3.0, 5.0, 7.0);

  test("vectors comparison", [&]{
    return vec == vec_copy;
  });

  test("vec + vec", [&]{
    return vec + vec2 == m::vec4(2.5, 2.5, 5.0, 7.1);
  });

  test("-vec", [&]{
    return -vec == m::vec4(-0.5, 0.5, 0.0, -0.1);
  });

  test("vec * scalar", [&]{
    return vec2 * -2.f == m::vec4(-4.0, -6.0, -10.0, -14.0);
  });

  test("vec * scalar", [&]{
    auto expected = m::vec4(2.0 * 0.5, -0.5 * 3.0, 0.0 * 5.0, 0.1 * 7.0);
    return vec * vec2 == expected;
  });

  test("scalar / vec", [&]{
    auto expected = m::vec4(10.0 / 2.0, 10.0 / 3.0, 10.0 / 5.0, 10.0 / 7.0);
    return 10.f / vec2 == expected;
  });

  test("vec.as_vec", [&]{
    return vec.as_vec<2>(0.f) == m::vec2(0.5, -0.5);
  });

  vec = m::vec4(-2.0, 3.0, -5.0, 7.0);
  test("vec.len_squred()", [&]{
    return vec.len_squared() == 87.0;
  });

  test("vec.len()", [&]{
    return vec.len() == std::sqrt(87.0);
  });

  test("normalization", []{
    auto vec = m::vec4(1.f);
    return vec.normalized() == vec / static_cast<float>(vec.len());
  });
 
  test("unit vector len", [&]{
    return m::compare<float>(vec.normalized().len(), 1.f);
  });

  test("dot product", [&]{
    auto [x1, y1, z1, w1] = vec;
    auto [x2, y2, z2, w2] = vec2;
    return m::dot(vec, vec2) == x1 * x2 + y1 * y2 + z1 * z2 + w1 * w2;
  });

  test("vec clamp()", [&]{
    const auto vec = m::vec4(-5.0, 4.0, 19.0, 3.33);
    const auto vec_min = m::vec4(2.0, -3.0, 1.5, -7.0);
    const auto vec_max = m::vec4(2.0, -0.5, 5.0, -1.0);
    return m::clamp(vec, vec_min, vec_max) == m::vec4(2.0, -0.5, 5.0, -1.0);
  });
  
  test("vec abs()", [&]{
    auto [x, y, z, w] = vec;

    return m::abs(vec) == m::vec4(
      m::abs(x),
      m::abs(y),
      m::abs(z),
      m::abs(w)
    );
  });

  test("vec3: cross product", [&]{
    const auto v1 = m::vec3(1.0, 2.0, 3.0);
    const auto v2 = m::vec3(98.0, 99.0, 100.0);
    const auto v3 = m::cross(v1, v2);

    return 
      v3 == m::vec3(-97.0, 194.0, -97.0) &&
      m::compare(m::dot(v1, v3), 0.f) &&
      m::compare(m::dot(v2, v3), 0.f);
  });

  std::cout << "ALL TESTS PASSED\n";
}
