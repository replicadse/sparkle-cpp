#include <iostream>

#include "util.hpp"
#include "geometry.hpp"
#include "scene.hpp"

using color3 = vec3<float>;

template <typename T>
color3 ray_color(const ray3<T>& r, const geometry<T>& world) {
    hit_rec<T> rec;
    if (world.hit(r, 0, infinity, rec)) {
        return 0.5f * (rec.normal + color3(1,1,1));
    }
    vec3<T> unit_direction = r.direction().unit();
    auto t = 0.5f*(unit_direction.y() + 1.0f);
    return (1.0f-t)*color3(1.0, 1.0, 1.0) + t*color3(0.5, 0.7, 1.0);
}

void write_color(std::ostream& out, color3 pixel_color) {
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

int main() {
    const auto aspect_ratio = 16.0f / 9.0f;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // World
    scene<float> world;
    world.add(make_shared<sphere<float>>(point3<float>(0,0,-1), 0.5f));
    world.add(make_shared<sphere<float>>(point3<float>(0,-100.5,-1), 100.0f));

    // Camera

    auto viewport_height = 2.0f;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0f;

    auto origin = point3<float>(0, 0, 0);
    auto horizontal = vec3<float>(viewport_width, 0, 0);
    auto vertical = vec3<float>(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3<float>(0, 0, focal_length);

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = float(i) / (image_width-1);
            auto v = float(j) / (image_height-1);
            ray3<float> r(origin, lower_left_corner + u*horizontal + v*vertical);
            color3 pixel_color = ray_color(r, world);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}
