# gf-math
Single-header c++ math library. The project is not finished yet. I will be adding more features in the future if I need something in my other projects.

## Overview:
### Ranges:
```cpp
namespace m = gf::math;

for (auto i : m::range(10)){
    std::cout << i << ' '; 
}
//Output: 0 1 2 3 4 5 6 7 8 9

for (auto i : m::range(5, 10)){
    std::cout << i << ' '; 
}
//Output: 5 6 7 8 9

```
For iterating through 2D structures (e.g matrices):

```cpp
namespace m = gf::math;

for (auto [x, y] : m::range({ 3, 3 })){
    std::cout << "(" << x << "," << y << ") ";
}
//Output: (0,0) (1,0) (2,0) (0,1) (1,1) (2,1) (0,2) (1,2) (2,2)

for (auto [x, y] : m::range({ 1, 1 }, { 3, 3 })){
    std::cout << "(" << x << "," << y << ") ";
}
//Output: (1,1) (2,1) (1,2) (2,2)
```

### Vectors:
Arithmetic operations:
```cpp
#define GEFEC_MATH_DEBUG
#include "math.hpp"
...
namespace m = gf::math;

const auto v1 = m::vec2(1.f, 2.f);
const auto v2 = m::vec2(3.f, 4.f);

std::cout << (v1 + v2) << '\n'; // [ 4 6 ]
std::cout << (v1 - v2) << '\n'; // [ -2 -2 ]
std::cout << (v1 * v2) << '\n'; // [ 3 8 ]
std::cout << (v1 + 5.f) << '\n'; // [ 6 7 ]
std::cout << m::dot(v1, v2) << '\n'; // 11 (dot product)

const auto v3 = m::vec3(1.f, 0.f, 0.f);
const auto v4 = m::vec3(0.f, 1.f, 0.f);

std::cout << m::cross(v3, v4) << '\n'; // [ 0 0 1 ] (cross product)
```
Structured binding supported:
```cpp
auto v = m::vec3(1.f, 2.f, 3.f);
auto& [x, y, z] = v;

x++;
z--;

std::cout << v; // [ 2 2 2 ]
```
Easy conversion:
```cpp
const auto v3 = m::vec3(1.f, 2.f, 3.f);
const auto v2 = v3.as_vec<2>(0.f);

std::cout << v2 << '\n'; // [ 1 2 ]

const auto v4 = v2.as_vec<4>(100.f);
std::cout << v4 << '\n'; // [ 1 2 100 100 ]
```
### Matrices
```cpp
const auto mat = m::mat2(
    3.f, 1.f,
    -1.f, 2.f
);

const auto det = mat.det(); //Matrix Determinant
const auto [a, c] = mat.data[0];
const auto [b, d] = mat.data[1];

const auto inv = 1.f / det * m::mat2(
    d, -b,
    -c, a
);

std::cout << mat * inv; //Identity matrix
```
Graphics calculations example:
```cpp
const auto aspect_ratio = 800.f / 600.f;
const auto fov = float(m::pi / 2.f); //90 degrees
const auto z_near = 0.1f;
const auto z_far = 1000.f;

const auto projection = m::perspective(aspect_ratio, fov, z_near, z_far);

const auto view = m::translation(m::vec3(0.f, 0.f, 15.f));

const auto angle = float(m::pi / 4.f);
const auto model
= m::scale(m::vec3(100.f))
* m::rotation(angle, m::vec3(1.f, 3.f, 0.f));

const auto point = m::vec3(0.f, 1.f, 0.f);

const auto projected = projection * view * model * point.as_vec<4>(1.f);
```
### Miscellaneous
Epsilon compare:
```cpp
const auto unit_vector = m::vec3(1.f).normalized();
std::cout << (unit_vector.len() == 1.f) << '\n'; //FALSE
std::cout << m::compare<float>(unit_vector.len(), 1.f) << '\n'; //TRUE
```
Other:
```cpp
const auto v1 = m::vec3(3.f, -2.f, 1.f);
const auto v2 = m::vec3(0.5f, -1.3f, 2.4f);

std::cout << m::abs(v1) << '\n'; // [ 3 2 1 ]

std::cout << m::round(v2) << '\n'; // [ 1 -1 2 ]
std::cout << m::floor(v2) << '\n'; // [ 0 -2 2 ]
std::cout << m::ceil(v2) << '\n'; // [ 1 -1 3 ]
std::cout << m::trunc(v2) << '\n'; // [ 0 -1 2 ]

std::cout << m::min(v1, v2) << '\n'; // [ 0.5 -2 1 ]
std::cout << m::max(v1, v2) << '\n'; // [ 3 -1.3 2.4 ]

const auto v3 = m::vec3(1.f, 2.f, 3.f);

std::cout << m::clamp(v1, v2, v3) << '\n'; // [ 1 -1.3 2.4 ]
```
