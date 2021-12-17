#ifndef GEOMETRY_H
#define GEOMETRY_H

template <typename T>
struct hit_rec {
  point3<T> p;
  vec3<T> normal;
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

template<typename T>
class sphere : public geometry<T> {
public:
  point3<T> m_center;
  T m_radius;

  sphere() {}
  sphere(const point3<T> center, T radius) {
    m_center = center;
    m_radius = radius;
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

    return true;
  }
};

#endif
