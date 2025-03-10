#define GEFEC_MATH_DEBUG
#include "../math.hpp"
#include "test.hpp"
#include <iomanip>

auto main() -> int{
  std::cerr << std::setprecision(100);

  using namespace gf::math;

  {
    auto vec = vec4(1.0, 2.0, 3.0, 4.0);
    auto mat = mat4(1.0);
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
    test("vec4");
    auto vec = vec4(1, 2, 3, 4);

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

    vec = vec4(-2, 3, -5, 7);

    auto vec2 = gf::math::vec<double, 4, false>(vec.dims);
    
    expect("len_squared()", vec.len_squared(), 87.0);
    expect("len()", vec.len(), std::sqrt(87.0));
    expect("no specialization: len_squared()", vec2.len_squared(), 87.0);
    expect("no specialization: len()", vec2.len(), std::sqrt(87.0));

    vec = vec4(1234.0);
    expect("single param ctor", vec.y, vec.x);
    expect("single param ctor", vec.z, vec.x);
    expect("single param ctor", vec.w, vec.x);

    const auto a = vec4(0.5, -0.5, 0.0, 0.1);
    const auto a_copy = a;
    const auto b = vec4(2.0, 3.0, 5.0, 7.0);

    expect("comparison", a == a_copy, true);
    expect("addition", a + b, vec4(2.5, 2.5, 5.0, 7.1));
    expect("negation", -a, vec4(-0.5, 0.5, 0.0, -0.1));
    expect("vec * scalar", b * -2.0, vec4(-4.0, -6.0, -10.0, -14.0));
    expect("vec * vec", a * b, vec4(2.0 * 0.5, -0.5 * 3.0, 0.0 * 5.0, 0.1 * 7.0));
    expect("scalar / vec", 10.0 / b, vec4(10.0 / 2.0, 10.0 / 3.0, 10.0 / 5.0, 10.0 / 7.0));
    expect("dot product", gf::math::dot_product(a, b), 0.5 * 2.0 - 0.5 * 3.0 + 7.0 * 0.1);
    vec = vec4(0.5);
    vec += 0.5;
    expect("normalization", vec.normalized(), vec / 2.0);
    expect("unit vector len", vec.normalized().len(), 1.0);

    const auto v1 = vec4(2.0, -0.5, 1.5, -1.0);
    const auto v2 = vec4(2.0, -3.0, 5.0, -7.0);
    expect("max", gf::math::max(v1, v2), vec4(2.0, -0.5, 5.0, -1.0));
    expect("min", gf::math::min(v1, v2), vec4(2.0, -3.0, 1.5, -7.0));
    expect("clamp", gf::math::clamp(v1, vec4(-1.0), vec4(1.0)), vec4(1.0, -0.5, 1.0, -1.0));
  }

  {
    test("vec10");
    auto vec = gf::math::vec<double, 10>(
      2.0, 3.0, 5.0, 7.0, 11.0, 13.0, 17.0, 19.0, 23.0, 29.0
    );

    auto mat = gf::math::mat<double, 10, 10>(1.0);

    expect("assign", vec.dims[0], 2.0);
    expect("assign", vec.dims[1], 3.0);
    expect("assign", vec.dims[9], 29.0);
    auto& [a, b, c, d, e, f, g, h, i, j] = vec;

    vec = gf::math::vec<double, 10>(11.11);
    const auto old_vec = vec;

    for (int i = 0; i < 10; ++i){
      std::cerr << i + 1 << ".";
      expect("single param ctor", vec.dims[i], 11.11);
    }

    vec *= mat;

    expect("identity 10x10 * vec10", vec, old_vec);
  }
}
