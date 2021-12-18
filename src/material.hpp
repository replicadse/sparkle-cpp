#ifndef MATERIAL_H
#define MATERIAL_H

using color3 = vec3<float>;

template <typename T>
struct hit_rec;
template <typename T>
vec3<T> random_unit_vector();

template <typename T>
class material {
public:
  virtual ~material() {}

  virtual bool scatter(const ray3<T>& r_in, const hit_rec<T>& rec, vec3<T>& attenuation, ray3<T>& scattered) const = 0;
};

template <typename T>
class lambertian_material : public material<T> {
public:
  color3 m_albedo;

  lambertian_material(const color3 a) : m_albedo(a) {}

  virtual bool scatter(const ray3<T>& r_in, const hit_rec<T>& rec, color3& attenuation, ray3<T>& scattered) const override {
    auto scatter_direction = rec.normal + random_unit_vector<T>();

    if (scatter_direction.length() < 1e-6) { // TODO performant enough? -> near zero component wise?
      scatter_direction = rec.normal;
    }

    scattered = ray3<T>(rec.p, scatter_direction);
    attenuation = m_albedo;
    return true;
  }
};

template <typename T>
class metal_material : public material<T> {
public:
  color3 m_albedo;

  metal_material(const color3& a): m_albedo(a) {}

  virtual bool scatter(const ray3<T>& r_in, const hit_rec<T>& rec, color3& attenuation, ray3<T>& scattered) const override {
    auto reflected = r_in.direction().unit().reflect(rec.normal);
    scattered = ray3<T>(rec.p, reflected);
    attenuation = m_albedo;
    return scattered.direction().dot(rec.normal) > 0;
  }
};

#endif
