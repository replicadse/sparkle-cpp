#include <iostream>

#include "util.hpp"
#include "geometry.hpp"
#include "scene.hpp"
#include "camera.hpp"
#include "material.hpp"

using color3 = vec3<float>;

template <typename T>
color3 ray_color(
  const ray3<T>& r, const geometry<T>& world,
  const int depth) {
    if (depth <= 0) {
      return color3(0, 0, 0);
    }

    hit_rec<T> rec;
    if (world.hit(r, 0.001, infinity, rec)) {
      ray3<T> scattered;
      color3 attenuation;
      if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return attenuation * ray_color<T>(scattered, world, depth-1);
      return color3(0, 0, 0);
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
    const float aspect_ratio = 16.0f / 10.0f;
    const int image_width = 640;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 32;
    const int max_depth = 32;

    auto cam_origin = point3<float>(3, 3, 2);
    auto cam_lookat = point3<float>(0, 0, -1);
    auto cam = camera(
      cam_origin,
      (cam_origin - cam_lookat).unit(),
      vec3<float>(0, 1, 0),
      degrees_to_radians(90),
      aspect_ratio,
      2.0,
      (cam_origin - cam_lookat).length()
    );

    // Materials
    auto material_ground = make_shared<lambertian_material<float>>(color3(0.8, 0.8, 0.0));
    auto material_center = make_shared<dielectric_material<float>>(1.7f);
    auto material_left   = make_shared<lambertian_material<float>>(color3(0.1, 0.2, 0.5));
    auto material_right  = make_shared<metal_material<float>>(color3(0.8, 0.6, 0.2), 0.2);

    // World
    geometry_group<float> world;
    world.add(make_shared<sphere<float>>(point3<float>( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere<float>>(point3<float>( 0.0,    0.0, -1.5),   0.5, material_center));
    world.add(make_shared<sphere<float>>(point3<float>(-1.0,    0.0, -4.0),   0.5, material_left));
    world.add(make_shared<sphere<float>>(point3<float>( 1.2,    0.0, -1.0),   0.5, material_right));

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int k = image_height-1; k >= 0; --k) {
        std::cerr << "\rScanlines remaining: " << k << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto col = color3(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
              auto u = (i + random_float()) / (image_width-1);
              auto v = (k + random_float()) / (image_height-1);
              ray3<float> r = cam.shoot(u, v);
              col += ray_color<float>(r, world, max_depth);
            }
            write_color(std::cout, col, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}
