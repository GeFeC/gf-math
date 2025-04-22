#define GEFEC_MATH_DEBUG
#include "../math.hpp"
#include "test.hpp"
#include <iomanip>

auto main() -> int{
  namespace m = gf::math;
  
  std::cerr << std::setprecision(100);

  static_assert(sizeof(m::mat<double, 10, 10>) == sizeof(double) * 10 * 10);

  test("mat: all operators implemented", []{
    auto mat = m::mat4(0.f);
    auto vec = m::vec4(1.0, 2.0, 3.0, 4.0);
    auto x = 1.f;

    mat = mat;

    mat == mat;
    mat != mat;

    mat + x;
    x + mat;
    mat + mat;

    -mat;
    mat - x;
    x - mat;
    mat - mat;

    mat * x;
    x * mat;
    mat * mat;
    mat * vec;
    vec * mat;

    mat / x;
    x / mat;
    mat / mat;
    mat / vec;
    vec / mat;

    mat += x;
    mat += mat;

    mat -= x;
    mat -= mat;

    mat *= x;
    mat /= x;

    return true;
  });

  auto mat = m::mat4(
    2.f, 3.f, 5.f, 7.f,
    11.f, 13.f, 17.f, 19.f,
    23.f, 29.f, 31.f, 37.f,
    41.f, 43.f, 47.f, 53.f
  );

  auto mat_copy = m::mat4(
    2.f, 3.f, 5.f, 7.f,
    11.f, 13.f, 17.f, 19.f,
    23.f, 29.f, 31.f, 37.f,
    41.f, 43.f, 47.f, 53.f
  );

  test("matrix default init", []{
    return m::mat4() == m::mat4(0.f);
  });

  test("matrix-matrix comparison", [&]{
    return mat == mat_copy;
  });

  mat_copy = mat;
  test("matrix copy", [&]{ 
    return mat_copy == mat; 
  });

  test("matrix transposition", [&]{ 
    return mat.t() == m::mat4(
      2.f, 11.f, 23.f, 41.f,
      3.f, 13.f, 29.f, 43.f,
      5.f, 17.f, 31.f, 47.f,
      7.f, 19.f, 37.f, 53.f
    );
  });

  test("transposition of transposition", [&]{
    return mat.t().t() == mat;
  });

  test("matrix + matrix", [&]{
    return mat + mat.t() == m::mat4(
      4.f, 14.f, 28.f, 48.f,
      14.f, 26.f, 46.f, 62.f,
      28.f, 46.f, 62.f, 84.f,
      48.f, 62.f, 84.f, 106.f
    );
  });

  test("mat.row", [&]{
    return 
      mat.row(0) == m::vec4(2.0, 3.0, 5.0, 7.0) &&
      mat.row(3) == m::vec4(41.0, 43.0, 47.0, 53.0);
  });

  test("mat.col", [&]{
    return
      mat.col(0) == m::vec4(2.0, 11.0, 23.0, 41.0) &&
      mat.col(3) == m::vec4(7.0, 19.0, 37.0, 53.0);
  });

  test("identity * mat = mat", [&]{
    return mat == m::mat4(1.f) * mat;
  });

  const auto mat1x4 = m::mat<float, 1, 4>(1.f, 6.f, 28.f, 496.f);

  test("mat4x4 * mat1x4", [&]{
    return mat * mat1x4 == m::mat<float, 1, 4>(
      2.f * 1.f + 3.f * 6.f + 5.f * 28.f + 7.f * 496.f,
      11.f * 1.f + 13.f * 6.f + 17.f * 28.f + 19.f * 496.f,
      23.f * 1.f + 29.f * 6.f + 31.f * 28.f + 37.f * 496.f,
      41.f * 1.f + 43.f * 6.f + 47.f * 28.f + 53.f * 496.f
    );
  });

  test("vec4 * mat1x4", [&]{
    auto vec = m::vec4(1.0, 2.0, 3.0, 4.0);
    return vec * mat1x4 == m::vec<float, 1>(1.f + 12.f + 3 * 28.f + 4.f * 496.f);
  });

  test("matrix clamp", [&]{
    const auto mat_min = m::mat4::filled(10.0);
    const auto mat_max = m::mat4::filled(25.0);
    
    return m::clamp(mat, mat_min, mat_max) == m::mat4(
      10.f, 10.f, 10.f, 10.f,
      11.f, 13.f, 17.f, 19.f,
      23.f, 25.f, 25.f, 25.f,
      25.f, 25.f, 25.f, 25.f
    );
  });

  test("vector translation", []{
    auto vec = m::vec4(0.0, 0.0, 0.0, 1.0);

    const auto translation = m::vec3(-2.0, 6.0, 4.0);
    return m::translation(translation) * vec == translation.as_vec<4>(1.f);
  });

  test("vector scale", [&]{ 
    auto scale = m::vec3(1.0, 2.0, 3.0);
    auto zero = m::vec4(0.0, 0.0, 0.0, 1.0);
    auto vec = m::vec4(1.0, 2.0, 3.0, 1.0);

    return 
      m::scale(scale) * zero == zero &&
      m::scale(scale) * vec == m::vec4(1.0, 4.0, 9.0, 1.0);
  });

  test("vector rotation", [&]{
    auto vec = m::vec4(0.0, 1.0, 0.0, 1.0);
    const auto rotation = m::rotation<float>(m::pi / 2.f, m::vec3(0.0, 0.0, 1.0)) * vec;

    return m::compare<float>(rotation, m::vec4(1.0, 0.0, 0.0, 1.0));
  });

  mat = m::mat4(
    1.f, 0.f, 0.f, 0.f,
    1.f, 1.f, 0.f, 0.f,
    1.f, 1.f, 1.f, 0.f,
    1.f, 1.f, 1.f, 1.f
  );

  auto mat2 = m::mat4(
    1.f, 1.f, 1.f, 1.f,
    0.f, 1.f, 1.f, 1.f,
    0.f, 0.f, 1.f, 1.f,
    0.f, 0.f, 0.f, 1.f
  );

  test("mat.is_lower_triangular()", [&]{
    return
      mat.is_lower_triangular() &&
      !mat2.is_lower_triangular();
  });

  test("mat.is_upper_triangular()", [&]{
    return
      !mat.is_upper_triangular() &&
      mat2.is_upper_triangular();
  });

  test("identity matrix is diagonal", [&]{
    return m::mat4(1.f).is_diagonal();
  });

  test("mat.det()", [&]{
    auto mat1 = m::mat4(0.f).map([i = 0.0](auto) mutable { return ++i; });

    auto mat2 = m::mat4(
      2.f, 3.f, 1.f, 4.f,
      0.f, 1.f, 5.f, 2.f,
      1.f, 0.f, 3.f, 1.f,
      4.f, 2.f, 1.f, 3.f
    );

    auto mat3 = m::mat4::filled(1.0);
    mat3[0][0] = 0;
    mat3[1][1] = 0;
    mat3[2][2] = 0;
    mat3[3][3] = 0;

    auto mat4 = m::mat4(
      0.f, 0.f, 3.f, 4.f,
      0.f, 0.f, 2.f, 2.f,
      4.f, 3.f, 2.f, 1.f,
      2.f, 2.f, 2.f, 4.f
    );

    auto mat5 = m::mat4(
      0.f, 0.f, 0.f, 1.f,
      0.f, 0.f, 1.f, 1.f,
      0.f, 1.f, 1.f, 1.f,
      1.f, 1.f, 1.f, 1.f
    );

    auto mat6 = m::mat4(
      1.f, 0.f, 0.f, 0.f,
      1.f, 1.f, 0.f, 0.f,
      1.f, 1.f, 1.f, 0.f,
      1.f, 1.f, 1.f, 1.f
    );

    auto mat7 = m::mat4(
      1.f, 1.f, 1.f, 1.f,
      1.f, 1.f, 1.f, 0.f,
      1.f, 1.f, 0.f, 0.f,
      1.f, 0.f, 0.f, 0.f
    );

    auto mat8 = m::mat4(
      0.f, 0.f, 0.f, 3.f,
      0.f, 0.f, 0.f, 2.f,
      0.f, 0.f, 0.f, 1.f,
      1.f, 2.f, 3.f, 4.f
    );

    return 
      m::mat4(0.f).det() == 0.f &&
      m::mat4(1.f).det() == 1.f &&
      mat1.det() == 0.f && 
      m::round(mat2.det()) == -16.f &&
      m::round(mat2.t().det()) == -16.0 &&
      mat3.det() == 0.0 &&
      m::round(mat4.det()) == -4.0 &&
      mat5.det() == 1.0 &&
      mat6.det() == 1.0 &&
      mat7.det() == 1.0 &&
      mat8.det() == 0.0;
  });

  std::cout << "ALL TESTS PASSED\n";
}
