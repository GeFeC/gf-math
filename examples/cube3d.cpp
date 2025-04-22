//WORKS UNDER LINUX ONLY!!!

#define GEFEC_MATH_DEBUG
#include "../math.hpp"
#include <iostream>
#include <string>
#include <utility>
#include <algorithm>
#include <thread>
#include <vector>

namespace m = gf::math;

static constexpr auto PixelWhite = "\u2588\u2588";
static constexpr auto PixelLightGray = "\u2593\u2593";
static constexpr auto PixelGray = "\u2592\u2592";
static constexpr auto PixelDark = "\u2591\u2591";

struct Renderer{
private:
  int width = 0, height = 0;
  std::string buffer;
  char null;

public:
  bool debug = false;
  m::mat4 model = m::mat4(1.f);
  m::mat4 view = m::mat4(1.f);
  m::mat4 projection = m::mat4(1.f);

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
        const auto pixel = at(m::ivec2(x, y));

        if (pixel == '0'){
          std::cout << PixelWhite;
        }
        else if (pixel == '1'){
          std::cout << PixelLightGray;
        }
        else if (pixel == '2'){
          std::cout << PixelGray;
        }
        else if (pixel == '3'){
          std::cout << PixelDark;
        }
        else std::cout << "  ";
      }
      std::cout << '\n';
    }
  }

  auto draw_point(const m::vec2& point){
    const auto center = size() / 2;

    auto position = center + m::ivec2(
      m::round(m::vec2(center) * point)
    );

    at(position) = '@';
  }

  auto make_line(const m::vec2& p1, const m::vec2& p2){
    auto points = std::vector<m::ivec2>();

    auto origin = m::ivec2(
      (p1 + 1.f) * m::vec2(width, height) / 2.f
    );

    auto target = m::ivec2(
      (p2 + 1.f) * m::vec2(width, height) / 2.f
    );

    if (origin.x > target.x) std::swap(origin, target);

    const auto [width, height] = m::abs(origin - target);
    const auto ratio = std::abs(static_cast<float>(width) / height);

    const auto direction = origin.y < target.y ? 1 : -1;

    if (ratio > 1.0){
      for (int i = 0; i != width; i++){
        points.push_back(origin + m::ivec2(i, static_cast<int>(i / ratio) * direction));
      }
    }
    else{
      for (int i = 0; i != height * direction; i += direction){
        points.push_back(origin + m::ivec2(static_cast<int>(i * ratio) * direction, i));
      }
    }

    points.push_back(target);

    return points;
  }

  auto project_point(const m::vec3& point){
    const auto projected_vec4 = projection * view * model * point.as_vec<4>(1.f);
    return projected_vec4.as_vec<2>(0.f) / projected_vec4.w;
  }

  auto draw_line(const m::vec2& p1, const m::vec2& p2){
    for (const auto& point : make_line(p1, p2)){
      at(point) = '@';
    }
  }

  auto draw_triangle(
    const m::vec3& p1,
    const m::vec3& p2,
    const m::vec3& p3
  ){
    const auto v1 = model * (p1 - p2).as_vec<4>(1.f);
    const auto v2 = model * (p1 - p3).as_vec<4>(1.f);
    const auto normal = m::cross(v1.as_vec<3>(0.f), v2.as_vec<3>(0.f));

    const auto point_3d = view * model * p1.as_vec<4>(1.f);
    if (m::dot(point_3d.as_vec<3>(0.f), normal) > 0.f) return;

    const auto projected = std::array{
      project_point(p1),
      project_point(p2),
      project_point(p3)
    };

    const auto l1 = make_line(projected[0], projected[1]);
    const auto l2 = make_line(projected[1], projected[2]);
    const auto l3 = make_line(projected[2], projected[0]);

    auto fill = std::unordered_map<double, std::vector<int>>();

    for (auto [x, y] : l1){
      fill[y].push_back(x);
    }

    for (auto [x, y] : l2){
      fill[y].push_back(x);
    }

    for (auto [x, y] : l3){
      fill[y].push_back(x);
    }

    const auto camera = m::vec3(0.0, 0.0, 1.0);
    const auto shade_color = m::abs(m::dot(camera, normal.normalized()));

    auto pixel = '0';
    if (shade_color < 0.75) pixel = '1';
    if (shade_color < 0.50) pixel = '2';
    if (shade_color < 0.25) pixel = '3';

    for (auto y : m::range(height)){
      if (fill.find(y) == fill.end()) continue;
      const auto& row = fill[y];

      const auto [fill_from, fill_to] = std::minmax_element(row.begin(), row.end());

      for (auto i : m::range(*fill_from, *fill_to)){
        at(m::ivec2(i, y)) = pixel;
      }
    }

  }
};

auto main() -> int{
  using namespace std::chrono_literals;

  std::cout << "Screen size: ";
  auto size = 0;
  std::cin >> size;

  auto renderer = Renderer(size, size);
  auto angle = 0.f;
  auto z = 10.f;
  for (;;){
    std::system("clear");
    renderer.clear();

    angle += 0.1;
    //z += 0.1;

    const auto sides = std::array{
      std::array{ //FRONT
        m::vec3(-0.5, -0.5, 0.5),
        m::vec3(0.5, -0.5, 0.5),
        m::vec3(0.5, 0.5, 0.5),
        m::vec3(-0.5, 0.5, 0.5)
      },
      std::array{ //BACK
        m::vec3(-0.5, 0.5, -0.5),
        m::vec3(0.5, 0.5, -0.5),
        m::vec3(0.5, -0.5, -0.5),
        m::vec3(-0.5, -0.5, -0.5)
      },
      std::array{ //LEFT
        m::vec3(-0.5, 0.5, 0.5),
        m::vec3(-0.5, 0.5, -0.5),
        m::vec3(-0.5, -0.5, -0.5),
        m::vec3(-0.5, -0.5, 0.5)
      },
      std::array{ //RIGHT
        m::vec3(0.5, -0.5, 0.5),
        m::vec3(0.5, -0.5, -0.5),
        m::vec3(0.5, 0.5, -0.5),
        m::vec3(0.5, 0.5, 0.5)
      },
      std::array{ //TOP
        m::vec3(-0.5, -0.5, -0.5),
        m::vec3(0.5, -0.5, -0.5),
        m::vec3(0.5, -0.5, 0.5),
        m::vec3(-0.5, -0.5, 0.5)
      },
      std::array{ //BOTTOM
        m::vec3(-0.5, 0.5, 0.5),
        m::vec3(0.5, 0.5, 0.5),
        m::vec3(0.5, 0.5, -0.5),
        m::vec3(-0.5, 0.5, -0.5)
      },
    };

    renderer.model = m::rotation(angle, m::vec3(0.f, -3.f, 1.f));
    renderer.view = m::translation(m::vec3(0.f, 0.f, z));
    renderer.projection = m::perspective(1.0, m::pi / 2.0, 0.1, 1000.0);

    for (auto i : m::range(sides.size())){
      const auto& side = sides[i];

      renderer.draw_triangle(side[0], side[1], side[2]);
      renderer.draw_triangle(side[2], side[3], side[0]);
    }

    renderer.render_buffer();

    std::this_thread::sleep_for(20ms);

    if (angle > m::pi * 2.0) angle -= m::pi * 2.0;
  }
}

