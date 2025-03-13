#define GEFEC_MATH_DEBUG
#include <iostream>
#include "../math.hpp"
#include "test.hpp"
#include <iomanip>

auto main() -> int{
  namespace m = gf::math;
  
  std::cerr << std::setprecision(100);

  static_assert(sizeof(m::mat<double, 10, 10>) == sizeof(double) * 10 * 10);

  {
    auto mat = m::mat4(1.0);
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
  }

  test("matrix");
  {
    expect("matrix init", m::mat4(), m::mat4(0.0));

    const auto mat = m::mat4(1.0);
    const auto mat2 = m::mat4(1.0);

    expect("matrix-matrix comparison", mat == mat2, true);
    const auto mat_copy = mat;
    expect("matrix copy", mat_copy, mat);
  }

  {
    const auto mat = m::mat3(
      2.0, 1.0, 3.0,
      7.0, 6.0, 9.0,
      4.0, 2.0, 0.0
    );

    expect("matrix 3x3 transposition", mat.t(), m::mat3(
      2.0, 7.0, 4.0,
      1.0, 6.0, 2.0,
      3.0, 9.0, 0.0
    ));
  }

  {
    const auto mat = m::mat4(
      2.0, 3.0, 5.0, 7.0,
      11.0, 13.0, 17.0, 19.0,
      23.0, 29.0, 31.0, 37.0,
      41.0, 43.0, 47.0, 53.0
    );

    expect("matrix 4x4 transposition", mat.t(), m::mat4(
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
  }

  {
    const auto mat = m::mat<double, 3, 3>(
      2.0, 1.0, 3.0,
      7.0, 6.0, 9.0,
      4.0, 2.0, 0.0
    );

    const auto mat2 = m::mat<double, 1, 3>(
      6.0,
      28.0,
      496.0
    );

    expect("mat3x3 * mat1x3", mat * mat2, m::mat<double, 1, 3>(
      12.0 + 28.0 + 3.0 * 496.0,
      42.0 + 6.0 * 28.0 + 9.0 * 496.0,
      24.0 + 56.0 + 0.0
    ));

    const auto mat5 = m::mat3().map([](auto){return 5.0;});
    expect("matrix clamp", m::clamp(mat, m::mat3(0.0), mat5), m::mat3(
      2.0, 1.0, 3.0,
      5.0, 5.0, 5.0,
      4.0, 2.0, 0.0
    ));
  }

  {
    const auto I = m::mat4(1.0);
    const auto mat = m::mat4(
      randval<double>(), randval<double>(), randval<double>(), randval<double>(),
      randval<double>(), randval<double>(), randval<double>(), randval<double>(),
      randval<double>(), randval<double>(), randval<double>(), randval<double>(),
      randval<double>(), randval<double>(), randval<double>(), randval<double>()
    );

    expect("identity * mat = mat", mat * I, mat);
  }

  {
    const auto vec = m::vec4(1.0, 2.0, 3.0, 4.0);
    
    const auto mat = m::mat4(
      randval<double>(), randval<double>(), randval<double>(), randval<double>(),
      randval<double>(), randval<double>(), randval<double>(), randval<double>(),
      randval<double>(), randval<double>(), randval<double>(), randval<double>(),
      randval<double>(), randval<double>(), randval<double>(), randval<double>()
    );

    expect("vec * mat == mat * vec", vec * mat, mat * vec);

    const auto mat1 = m::mat<double, 1, 4>(
      2.0, 1.0, 3.0, 7.0
    );

    expect("vec4 * mat1x4", vec * mat1, m::vec<double, 1>(41));
  }

  {
    const auto mat = m::mat4(
      1.0, 0.0, 0.0, 0.0,
      1.0, 1.0, 0.0, 0.0,
      1.0, 1.0, 1.0, 0.0,
      1.0, 1.0, 1.0, 1.0
    );

    expect("mat.is_lower_triangular()", mat.is_lower_triangular(), true);
    expect("mat.is_upper_triangular()", mat.is_upper_triangular(), false);

    const auto mat2 = m::mat4(
      1.0, 1.0, 1.0, 1.0,
      0.0, 1.0, 1.0, 1.0,
      0.0, 0.0, 1.0, 1.0,
      0.0, 0.0, 0.0, 1.0
    );

    expect("mat.is_lower_triangular()", mat2.is_lower_triangular(), false);
    expect("mat.is_upper_triangular()", mat2.is_upper_triangular(), true);

    expect("identity mat is diagonal", m::mat4(1.0).is_diagonal(), true);
  }

  {
    const auto mat = m::mat4(0.0).map([i=0.0](auto) mutable { return ++i; });
    expect("matrix without 0's det()", mat.det(), 0.0);

    const auto i = m::mat4(1.0);
    expect("identity mat.det()", i.det(), 1.0);

    const auto zero_mat = m::mat4(0.0);
    expect("zero mat.det()", zero_mat.det(), 0.0);

    const auto mat2 = m::mat4(
      2.0, 3.0, 1.0, 4.0,
      0.0, 1.0, 5.0, 2.0,
      1.0, 0.0, 3.0, 1.0,
      4.0, 2.0, 1.0, 3.0
    );

    expect("non zero det", std::round(mat2.det()), -16.0);
    expect("mat.t().det()", std::round(mat2.t().det()), -16.0);

    auto zero_diagonal = m::mat4(0.0).map([](auto){ return 1.0; });
    zero_diagonal[0][0] = 0;
    zero_diagonal[1][1] = 0;
    zero_diagonal[2][2] = 0;
    zero_diagonal[3][3] = 0;

    expect("zero diagonal mat.det()", zero_diagonal.det(), 0.0);

    const auto mat3 = m::mat4(
      0.0, 0.0, 3.0, 4.0,
      0.0, 0.0, 2.0, 2.0,
      4.0, 3.0, 2.0, 1.0,
      2.0, 2.0, 2.0, 4.0
    );

    expect("mat with 2x2 square of 0's det", std::round(mat3.det()), -4.0);

    const auto mat4 = m::mat4(
      0.0, 0.0, 0.0, 1.0,
      0.0, 0.0, 1.0, 1.0,
      0.0, 1.0, 1.0, 1.0,
      1.0, 1.0, 1.0, 1.0
    );

    expect("other diagonal mat det", mat4.det(), 1.0);
 
    const auto mat5 = m::mat4(
      1.0, 0.0, 0.0, 0.0,
      1.0, 1.0, 0.0, 0.0,
      1.0, 1.0, 1.0, 0.0,
      1.0, 1.0, 1.0, 1.0
    );

    expect("0's above diagonal mat det", mat5.det(), 1.0); 

    const auto mat6 = m::mat4(
      1.0, 1.0, 1.0, 1.0,
      1.0, 1.0, 1.0, 0.0,
      1.0, 1.0, 0.0, 0.0,
      1.0, 0.0, 0.0, 0.0
    );

    expect("0's above other diagonal mat det", mat6.det(), 1.0); 

    const auto mat7 = m::mat4(
      0.0, 0.0, 0.0, 3.0,
      0.0, 0.0, 0.0, 2.0,
      0.0, 0.0, 0.0, 1.0,
      1.0, 2.0, 3.0, 4.0
    );

    expect("more 0's mat det", std::round(mat7.det()), 0.0);
  }
}
