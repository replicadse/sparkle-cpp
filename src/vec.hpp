#ifndef VEC3_H
#define VEC3_H

template <typename T>
class vec3 {
public:
  T e[3];

  vec3(){}
  vec3(const T e0, const T e1, const T e2) {
    e[0] = e0;
    e[1] = e1;
    e[2] = e2;
  }

  inline T x() const { return e[0]; }
  inline T y() const { return e[1]; }
  inline T z() const { return e[2]; }

  inline vec3 operator+(const vec3 &v) const {
      return vec3(e[0] + v.e[0], e[1] + v.e[1], e[2] + v.e[2]);
  }
  inline vec3 operator-(const vec3 &v) const {
      return vec3(e[0] - v.e[0], e[1] - v.e[1], e[2] - v.e[2]);
  }
  inline vec3 operator*(const vec3 &v) const {
      return vec3(e[0] * v.e[0], e[1] * v.e[1], e[2] * v.e[2]);
  }
  inline vec3 operator*(const T t) const {
      return vec3(t*e[0], t*e[1], t*e[2]);
  }
  inline vec3 operator/(const T t) const {
    T inv = 1/t;
    return vec3(e[0]*inv, e[1]*inv, e[2]*inv);
  }

  inline vec3& operator+=(const T t) {
    e[0] += t;
    e[1] += t;
    e[2] += t;
    return *this;
  }
  inline vec3& operator+=(const vec3 &v) {
    e[0] += v[0];
    e[1] += v[1];
    e[2] += v[2];
    return *this;
  }
  inline vec3& operator-=(const vec3 &v) {
    e[0] -= v[0];
    e[1] -= v[1];
    e[2] -= v[2];
  }
  inline vec3& operator*=(const T t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
  }
  inline vec3& operator/=(const T t) {
    return *this *= 1/t;
  }

  inline T operator[](const int i) const { return e[i]; }
  inline T& operator[](const int i) { return e[i]; }

  inline T length() const {
    return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
  }
  inline T length_sqr() const {
    return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
  }

  inline vec3 unit() const {
    auto l = length();
    return vec3(e[0]/l, e[1]/l, e[2]/l);
  }
  inline void to_unit() {
    *this /= length();
  }

  inline T dot(const vec3 &v) const {
    return e[0] * v.e[0]
         + e[1] * v.e[1]
         + e[2] * v.e[2];
  }
  inline vec3 cross(const vec3 &v) const {
      return vec3(e[1] * v.e[2] - e[2] * v.e[1],
                  e[2] * v.e[0] - e[0] * v.e[2],
                  e[0] * v.e[1] - e[1] * v.e[0]);
  }
};

template <typename T>
using point3 = vec3<T>;

template <typename T>
inline std::ostream& operator<<(std::ostream &out, const vec3<T> &v) {
    return out << '[' << v.e[0] << ", " << v.e[1] << ", " << v.e[2] << ']';
}

template <typename T>
inline vec3<T> operator*(T t, const vec3<T> &v) {
  return v * t;
}

#endif
