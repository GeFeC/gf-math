#define GEFEC_MATH_DEBUG
#include "../math.hpp"
#include "test.hpp"

auto main() -> int{
  namespace m = gf::math;
  
  //std::cerr << std::setprecision(100);

  static_assert(sizeof(m::mat<double, 10, 10>) == sizeof(double) * 10 * 10);
  auto mat = m::mat4(0.0);
  auto vec = m::vec4(1.0, 2.0, 3.0, 4.0);
  auto x = 1.0;

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

  mat = m::mat4(
    2.0, 3.0, 5.0, 7.0,
    11.0, 13.0, 17.0, 19.0,
    23.0, 29.0, 31.0, 37.0,
    41.0, 43.0, 47.0, 53.0
  );

  auto mat_copy = m::mat4(
    2.0, 3.0, 5.0, 7.0,
    11.0, 13.0, 17.0, 19.0,
    23.0, 29.0, 31.0, 37.0,
    41.0, 43.0, 47.0, 53.0
  );


  test("matrix");
  expect("matrix init", m::mat4(), m::mat4(0.0));

  expect("matrix-matrix comparison", mat == mat_copy, true);
  mat_copy = mat;
  expect("matrix copy", mat_copy, mat);

  expect("matrix transposition", mat.t(), m::mat4(
    2.0, 11.0, 23.0, 41.0,
    3.0, 13.0, 29.0, 43.0,
    5.0, 17.0, 31.0, 47.0,
    7.0, 19.0, 37.0, 53.0
  ));

  expect("transposition of transposition", mat.t().t(), mat);

  expect("matrix + matrix", mat + mat.t(), m::mat4(
    4.0, 14.0, 28.0, 48.0,
    14.0, 26.0, 46.0, 62.0,
    28.0, 46.0, 62.0, 84.0,
    48.0, 62.0, 84.0, 106.0
  ));

  expect("mat.row(0)", mat.row(0), m::vec4(2.0, 3.0, 5.0, 7.0));
  expect("mat.row(3)", mat.row(3), m::vec4(41.0, 43.0, 47.0, 53.0));

  expect("mat.col(0)", mat.col(0), m::vec4(2.0, 11.0, 23.0, 41.0));
  expect("mat.col(3)", mat.col(3), m::vec4(7.0, 19.0, 37.0, 53.0));

  const auto mat1x4 = m::mat<double, 1, 4>(
    1.0,
    6.0,
    28.0,
    496.0
  );

  expect("mat4x4 * mat1x4", mat * mat1x4, m::mat<double, 1, 4>(
    2.0 * 1.0 +3.0 * 6.0 +5.0 * 28.0 +7.0 * 496.0,
    11.0 * 1.0 +13.0 * 6.0 +17.0 * 28.0 +19.0 * 496.0,
    23.0 * 1.0 +29.0 * 6.0 +31.0 * 28.0 +37.0 * 496.0,
    41.0 * 1.0 +43.0 * 6.0 +47.0 * 28.0 +53.0 * 496.0
  ));

  const auto mat_max = m::mat4().map([](auto){ return 25.0; });
  const auto mat_min = m::mat4().map([](auto){ return 10.0; });
  expect("matrix clamp", m::clamp(mat, mat_min, mat_max), m::mat4(
    10.0, 10.0, 10.0, 10.0,
    11.0, 13.0, 17.0, 19.0,
    23.0, 25.0, 25.0, 25.0,
    25.0, 25.0, 25.0, 25.0
  ));

  expect("identity * mat = mat", mat * m::mat4(1.0), mat);

  vec = m::vec4(1.0, 2.0, 3.0, 4.0);

  expect("vec * mat == mat * vec", vec * mat, mat * vec);

  expect("vec4 * mat1x4", vec * mat1x4, m::vec<double, 1>(1.0 + 12.0 + 3 * 28.0 + 4.0 * 496.0));

  vec = m::vec4(0.0, 0.0, 0.0, 1.0);

  const auto translation = m::vec3(-2.0, 6.0, 4.0);
  expect("vector translation", vec * m::translation(translation), m::vec4(-2.0, 6.0, 4.0, 1.0));
  expect("zero vector scale", vec * m::scale(m::vec3(1.0)), vec);
  vec = m::vec4(1.0, 2.0, 3.0, 1.0);
  const auto scale = m::vec3(1.0, 2.0, 3.0);
  expect("vector scale", vec * m::scale(scale), m::vec4(1.0, 4.0, 9.0, 1.0));
  vec = m::vec4(0.0, 1.0, 0.0, 1.0);

  mat = m::mat4(
    1.0, 0.0, 0.0, 0.0,
    1.0, 1.0, 0.0, 0.0,
    1.0, 1.0, 1.0, 0.0,
    1.0, 1.0, 1.0, 1.0
  );

  expect("mat.is_lower_triangular()", mat.is_lower_triangular(), true);
  expect("mat.is_upper_triangular()", mat.is_upper_triangular(), false);

  mat = m::mat4(
    1.0, 1.0, 1.0, 1.0,
    0.0, 1.0, 1.0, 1.0,
    0.0, 0.0, 1.0, 1.0,
    0.0, 0.0, 0.0, 1.0
  );

  expect("mat.is_lower_triangular()", mat.is_lower_triangular(), false);
  expect("mat.is_upper_triangular()", mat.is_upper_triangular(), true);

  expect("identity mat is diagonal", m::mat4(1.0).is_diagonal(), true);

  mat = m::mat4(0.0).map([i=0.0](auto) mutable { return ++i; });
  expect("matrix without 0's det()", mat.det(), 0.0);

  expect("identity mat.det()", m::mat4(1.0).det(), 1.0);

  expect("zero mat.det()", m::mat4(0.0).det(), 0.0);

  mat = m::mat4(
    2.0, 3.0, 1.0, 4.0,
    0.0, 1.0, 5.0, 2.0,
    1.0, 0.0, 3.0, 1.0,
    4.0, 2.0, 1.0, 3.0
  );

  expect("non zero det", std::round(mat.det()), -16.0);
  expect("mat.t().det()", std::round(mat.t().det()), -16.0);

  mat = m::mat4(0.0).map([](auto){ return 1.0; });
  mat[0][0] = 0;
  mat[1][1] = 0;
  mat[2][2] = 0;
  mat[3][3] = 0;

  expect("zero diagonal mat.det()", mat.det(), 0.0);

  mat = m::mat4(
    0.0, 0.0, 3.0, 4.0,
    0.0, 0.0, 2.0, 2.0,
    4.0, 3.0, 2.0, 1.0,
    2.0, 2.0, 2.0, 4.0
  );

  expect("mat with 2x2 square of 0's det", std::round(mat.det()), -4.0);

  mat = m::mat4(
    0.0, 0.0, 0.0, 1.0,
    0.0, 0.0, 1.0, 1.0,
    0.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0
  );

  expect("other diagonal mat det", mat.det(), 1.0);

  mat = m::mat4(
    1.0, 0.0, 0.0, 0.0,
    1.0, 1.0, 0.0, 0.0,
    1.0, 1.0, 1.0, 0.0,
    1.0, 1.0, 1.0, 1.0
  );

  expect("0's above diagonal mat det", mat.det(), 1.0); 

  mat = m::mat4(
    1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 0.0,
    1.0, 1.0, 0.0, 0.0,
    1.0, 0.0, 0.0, 0.0
  );

  expect("0's above other diagonal mat det", mat.det(), 1.0); 

  mat = m::mat4(
    0.0, 0.0, 0.0, 3.0,
    0.0, 0.0, 0.0, 2.0,
    0.0, 0.0, 0.0, 1.0,
    1.0, 2.0, 3.0, 4.0
  );

  expect("more 0's mat det", std::round(mat.det()), 0.0);
}
