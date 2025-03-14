#define GEFEC_MATH_DEBUG
#include "../math.hpp"
#include "test.hpp"
#include <iomanip>

auto main() -> int{
  std::cerr << std::setprecision(100);

  namespace m = gf::math;

  static_assert(sizeof(m::vec<double, 10>) == sizeof(double) * 10);

  {
    auto vec = m::vec4(1.0, 2.0, 3.0, 4.0);
    auto mat = m::mat4(1.0);
    auto x = 1.0;

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
  }

  {
    test("vec");
    expect("vec init", m::vec4(), m::vec4(0.0, 0.0, 0.0, 0.0));

    auto vec = m::vec4(1.0, 2.0, 3.0, 4.0);

    expect("x assign", vec.x, 1.0);
    expect("y assign", vec.y, 2.0);
    expect("z assign", vec.z, 3.0);
    expect("w assign", vec.w, 4.0);

    expect("x == r", vec.x, vec.r);
    expect("y == g", vec.y, vec.g);
    expect("z == b", vec.z, vec.b);
    expect("w == a", vec.w, vec.a);

    auto& [x, y, z, w] = vec;

    expect("struct binding x", &vec.x, &x);
    expect("struct binding y", &vec.y, &y);
    expect("struct binding z", &vec.z, &z);
    expect("struct binding w", &vec.w, &w);

    vec = m::vec4(-2.0, 3.0, -5.0, 7.0);
    
    expect("len_squared()", vec.len_squared(), 87.0);
    expect("len()", vec.len(), std::sqrt(87.0));
    expect("scalar abs()", gf::math::abs(-4.0), 4.0);
    expect("vec abs()", gf::math::abs(vec), m::vec4(2.0, 3.0, 5.0, 7.0));

    vec = m::vec4(1234.0);
    expect("single param ctor", vec.y, vec.x);
    expect("single param ctor", vec.z, vec.x);
    expect("single param ctor", vec.w, vec.x);

    const auto a = m::vec4(0.5, -0.5, 0.0, 0.1);
    const auto a_copy = a;
    const auto b = m::vec4(2.0, 3.0, 5.0, 7.0);

    expect("comparison", a == a_copy, true);
    expect("addition", a + b, m::vec4(2.5, 2.5, 5.0, 7.1));
    expect("negation", -a, m::vec4(-0.5, 0.5, 0.0, -0.1));
    expect("vec * scalar", b * -2.0, m::vec4(-4.0, -6.0, -10.0, -14.0));
    expect("vec * vec", a * b, m::vec4(2.0 * 0.5, -0.5 * 3.0, 0.0 * 5.0, 0.1 * 7.0));
    expect("scalar / vec", 10.0 / b, m::vec4(10.0 / 2.0, 10.0 / 3.0, 10.0 / 5.0, 10.0 / 7.0));
    expect("dot product", m::dot(a, b), 0.5 * 2.0 - 0.5 * 3.0 + 7.0 * 0.1);
    vec = m::vec4(0.5);
    vec += 0.5;
    expect("normalization", vec.normalized(), vec / 2.0);
    expect("unit vector len", vec.normalized().len(), 1.0);

    vec = m::vec4(-5.0, 4.0, 19.0, 3.33);

    const auto v1 = m::vec4(2.0, -3.0, 1.5, -7.0);
    const auto v2 = m::vec4(2.0, -0.5, 5.0, -1.0);
    expect("scalar clamp", m::clamp(1.5, -1.0, 1.0), 1.0);
    expect("vec clamp", m::clamp(vec, v1, v2), m::vec4(2.0, -0.5, 5.0, -1.0));
  }

  {
    test("vec3");

    auto vec = m::vec3(1.0, 2.0, 3.0);
    auto vec2 = m::vec3(98.0, 99.0, 100.0);

    expect("cross product", cross(vec, vec2), m::vec3(-97.0, 194.0, -97.0));
  }
}
