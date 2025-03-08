#define GEFEC_MATH_DEBUG
#include "../math.hpp"
#include "test.hpp"

auto main() -> int{
  namespace m = gf::math;

  test("matrix");
  {
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
}
