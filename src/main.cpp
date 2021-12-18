#include <iostream>

#include "util.hpp"
#include "geometry.hpp"
#include "scene.hpp"
#include "camera.hpp"

using color3 = vec3<float>;

enum DIFFUSE_MATERIAL_RENDER_METHOD {
  DMRM_LAMBERTIAN_REFLECTION,
  DMRM_TRUE_LAMBERTIAN_REFLECTION,
  DMRM_RANDOM_ANGLE,
};
const int USED_DIFFUSE_RENDER_METHOD = DMRM_RANDOM_ANGLE;

template <typename T>
color3 ray_color(
  const ray3<T>& r, const geometry<T>& world,
  const int depth) {
    const float REFLECTIVENESS = 0.4f;
    if (depth <= 0) {
      return color3(0, 0, 0);
    }

    hit_rec<T> rec;
    if (world.hit(r, 0.001, infinity, rec)) {
        point3<T> target = rec.p + rec.normal;
        switch (USED_DIFFUSE_RENDER_METHOD) {
          case DMRM_LAMBERTIAN_REFLECTION:
            target += random_in_unit_sphere<T>();
            break;
          case DMRM_TRUE_LAMBERTIAN_REFLECTION:
            target += random_unit_vector<T>();
            break;
          case DMRM_RANDOM_ANGLE:
            target += random_in_hemisphere<T>(rec.normal);
            break;
          default:
            target += random_in_hemisphere<T>(rec.normal);
        }
        return REFLECTIVENESS * ray_color(ray3<T>(rec.p, target - rec.p), world, depth-1);
    }
    vec3<T> unit_direction = r.direction().unit();
    auto t = 0.5f*(unit_direction.y() + 1.0f);
    return (1.0f-t)*color3(1.0, 1.0, 1.0) + t*color3(0.5, 0.7, 1.0);
}

void write_color(std::ostream& out, color3 pixel_color, int samples_per_pixel) {
  auto sc = pixel_color / samples_per_pixel;
  // gamma correction for gamma=2.0f
  sc = color3(sqrt(sc.x()), sqrt(sc.y()), sqrt(sc.z()));

  out << static_cast<int>(256 * clamp<float>(sc.x(), 0.0, 0.99)) << ' '
      << static_cast<int>(256 * clamp<float>(sc.y(), 0.0, 0.99)) << ' '
      << static_cast<int>(256 * clamp<float>(sc.z(), 0.0, 0.99)) << '\n';
}

int main() {
    const float aspect_ratio = 16.0f / 9.0f;
    // const float aspect_ratio = 1.0f / 1.0f;
    const int image_width = 640;
    // const int image_width = 2;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 32;
    const int max_depth = 32;

    auto cam = camera(aspect_ratio*2, 2, 1.0f);

    // World
    geometry_group<float> world;
    world.add(make_shared<sphere<float>>(point3<float>(0,0,-1), 0.5f));
    world.add(make_shared<sphere<float>>(point3<float>(0,-100.5,-1), 100.0f));

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int k = image_height-1; k >= 0; --k) {
        std::cerr << "\rScanlines remaining: " << k << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto col = color3(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
              auto u = (i + random_float()) / (image_width-1);
              auto v = (k + random_float()) / (image_height-1);
              ray3<float> r = cam.shoot(u, v);
              col += ray_color(r, world, max_depth);
            }
            write_color(std::cout, col, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}
