#ifndef MATERIAL_H
#define MATERIAL_H

using color3 = vec3<float>;

template <typename T>
struct hit_rec;
template <typename T>
vec3<T> random_unit_vector();
template <typename T>
vec3<T> random_in_unit_sphere();

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
  T m_fuzz;

  metal_material(const color3& a, const T fuzz): m_albedo(a), m_fuzz(clamp<T>(fuzz, 0, 1)) {}

  virtual bool scatter(const ray3<T>& r_in, const hit_rec<T>& rec, color3& attenuation, ray3<T>& scattered) const override {
    auto reflected = r_in.direction().unit().reflect(rec.normal);
    scattered = ray3<T>(rec.p, reflected + m_fuzz*random_in_unit_sphere<T>());
    attenuation = m_albedo;
    return scattered.direction().dot(rec.normal) > 0;
  }
};

template <typename T>
class dielectric_material : public material<T> {
public:
  T m_refraction;
  dielectric_material(const T refraction) : m_refraction(refraction) { }

  virtual bool scatter(const ray3<T>& r_in, const hit_rec<T>& rec, color3& attenuation, ray3<T>& scattered) const override {
    attenuation = color3(1, 1, 1);
    T refrac_ratio = rec.front_face ? (1.0f / m_refraction) : m_refraction;
    vec3<T> unit_direction = r_in.direction().unit();

    T cos_theta = fmin((unit_direction * -1).dot(rec.normal), 1.0f);
    T sin_theta = static_cast<T>(sqrt(1.0f - cos_theta*cos_theta));
    auto must_reflect = refrac_ratio * sin_theta > 1.0f;
    vec3<T> direction;
    if (must_reflect || reflectance<T>(cos_theta, refrac_ratio) > random_float())
      direction = unit_direction.reflect(rec.normal);
    else
      direction = unit_direction.refract(rec.normal, refrac_ratio);

    scattered = ray3<T>(rec.p, direction);
    return true;
  }

private:
  template <typename TV>
  static TV reflectance(TV cosine, TV ref) {
    auto r0 = static_cast<TV>((1-ref) / (1+ref));
    return r0 + (1-r0)*pow((1-cosine), 5);
  }
};

#endif
