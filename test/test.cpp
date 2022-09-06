#include "../gf-math.hpp"

#include <cassert>

auto main() -> int{
  auto [x, y] = gf::vec2(21, 37);
  assert(x == 21 && y == 37);

  auto v = gf::vec2(21, 37);
  
  v = -v;
  assert(v == gf::vec2(-21, -37));

  v = v + gf::vec2(6, 9);
  assert(v == gf::vec2(-21 + 6, -37 + 9));

  //TODO more tests!
}