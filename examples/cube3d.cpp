#define GEFEC_MATH_DEBUG
#include "../math.hpp"
#include <iostream>
#include <string>
#include <utility>
#include <thread>
#include <vector>

namespace m = gf::math;

struct Renderer{
private:
  int width = 0, height = 0;
  std::string buffer;
  char null;

public:
  Renderer(int width, int height) : width(width), height(height){
    buffer.resize(width * height, ' ');
  }

  auto& at(const m::ivec2& coords){
    const auto [x, y] = coords;

    if (y < 0 || y > height) return null;
    if (x < 0 || x > width) return null;

    return buffer[y * width + x];
  }

  const auto& at(const m::ivec2& coords) const{
    return const_cast<Renderer*>(this)->at(coords);
  }

  auto clear(){
    buffer = std::string(width * height, ' ');
  }

  auto size() const{
    return m::ivec2(width, height);
  }

  auto render_buffer(){
    for (int y = 0; y < height; ++y){
      for (int x = 0; x < width; ++x){
        if (at(m::ivec2(x, y)) != ' '){
          std::cout << "\u2588\u2588";
        }
        else std::cout << "  ";
      }
      std::cout << '\n';
    }
  }

  auto draw_point(const m::vec2& point){
    const auto center = size() / 2;

    auto position = center + m::ivec2(
      std::round(center.x * point.x),
      std::round(center.y * point.y)
    );

    at(position) = '@';
  }

inline auto draw_line(
  const m::vec2& p1, 
  const m::vec2& p2
){
  auto origin = m::ivec2(
    (p1 + 1.0) * m::vec2(width, height) / 2.0
  );

  auto target = m::ivec2(
    (p2 + 1.0) * m::vec2(width, height) / 2.0
  );

  if (origin.x > target.x) std::swap(origin, target);

  at(origin) = '@';
  at(target) = '@';

  const auto [width, height] = m::abs(origin - target);
  const auto ratio = std::abs(static_cast<float>(width) / height);

  const auto direction = origin.y < target.y ? 1 : -1;

  if (ratio > 1.0){
    for (int i = 0; i != width; i++){
      at(origin + m::ivec2(i, static_cast<int>(i / ratio) * direction)) = '@';
    }
  }
  else{
    for (int i = 0; i != height * direction; i += direction){
      at(origin + m::ivec2(static_cast<int>(i * ratio) * direction, i)) = '@';
    }
  }
}

};

auto main() -> int{
  auto renderer = Renderer(50, 50);
  using namespace std::chrono_literals;

  auto angle = 0.0;
  for (;;){
    angle += 0.1;
    std::system("clear");
    renderer.clear();

    const auto cube_front = std::vector{
      m::vec3(-0.5, -0.5, 0.5),
      m::vec3(0.5, -0.5, 0.5),
      m::vec3(0.5, 0.5, 0.5),
      m::vec3(-0.5, 0.5, 0.5)
    };

    const auto cube_back = std::vector{
      m::vec3(-0.5, -0.5, -0.5),
      m::vec3(0.5, -0.5, -0.5),
      m::vec3(0.5, 0.5, -0.5),
      m::vec3(-0.5, 0.5, -0.5)
    };

    const auto model = m::rotation(angle, m::vec3(0.0, 3.0, 1.0));
    const auto view = m::translation(m::vec3(0.0, 0.0, 10.0));
    const auto projection = m::perspective(1.0, m::pi / 2.0, 0.1, 10000.0);
    const auto mvp = projection * view * model;

    const auto render_line = [&](const m::vec3& v1, const m::vec3& v2){
      const auto v1_projected = mvp * v1.as_vec<4>(1.0);
      const auto v2_projected = mvp * v2.as_vec<4>(1.0);

      const auto v1_projected_2d = v1_projected.as_vec<2>() / v1_projected.w;
      const auto v2_projected_2d = v2_projected.as_vec<2>() / v2_projected.w;

      renderer.draw_line(v1_projected_2d, v2_projected_2d);
    };

    for (auto i : m::range(3)){
      render_line(cube_front[i], cube_front[i + 1]);
      render_line(cube_back[i], cube_back[i + 1]);
    }

    for (auto i : m::range(4)){
      render_line(cube_front[i], cube_back[i]);
    }

    render_line(cube_front[0], cube_front[3]);
    render_line(cube_back[0], cube_back[3]);

    renderer.render_buffer();
    std::this_thread::sleep_for(50ms);
  }
}

