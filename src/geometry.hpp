#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "material.hpp"

template <typename T>
class material;

template <typename T>
struct hit_rec {
  point3<T> p;
  vec3<T> normal;
  shared_ptr<material<T>> mat_ptr;
  T t;
  bool front_face;

  inline void set_face_normal(const ray3<T> r, const vec3<T>& outward_normal) {
    front_face = r.direction().dot(outward_normal) < 0;
    normal = front_face ? outward_normal : outward_normal * -1;
  }
};

template <typename T>
class geometry {
public:
  virtual ~geometry() {};
  virtual bool hit(const ray3<T>& r, const T t_min, const T t_max, hit_rec<T>& hit) const = 0;
};

template <typename T>
class sphere : public geometry<T> {
public:
  point3<T> m_center;
  T m_radius;
  shared_ptr<material<T>> m_material_ptr;

  sphere() {}
  sphere(const point3<T> center, T radius, shared_ptr<material<T>> material_ptr) {
    m_center = center;
    m_radius = radius;
    m_material_ptr = material_ptr;
  }

  virtual bool hit(const ray3<T>& r, const T t_min, const T t_max, hit_rec<T>& hit) const override {
    vec3<T> oc = r.origin() - m_center;
    auto a = r.direction().length_sqr();
    auto half_b = oc.dot(r.direction());
    auto c = oc.length_sqr() - m_radius*m_radius;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0)
      return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    hit.t = root;
    hit.p = r.at(hit.t);
    auto outward_normal = (hit.p - m_center) / m_radius;
    hit.set_face_normal(r, outward_normal);
    hit.mat_ptr = m_material_ptr;

    return true;
  }
};

template <typename T>
vec3<T> random_in_unit_sphere() {
  while (true) {
    auto p = vec3<T>::random(-1, 1);
    if (p.length_sqr() >= 1) continue;
    return p;
  }
}

template <typename T>
vec3<T> random_unit_vector() {
  return random_in_unit_sphere<T>().unit();
}

template <typename T>
vec3<T> random_in_hemisphere(const vec3<T>& normal) {
  auto vius = random_in_unit_sphere<T>();
  if (vius.dot(normal) > 0.0f) // It is in the same hemisphere as the normal
    return vius;
  else
    return vius * -1;
}

#endif
