#ifndef RAY_H
#define RAY_H

#include <iostream>
#include "vec.hpp"

template <typename T>
class ray3 {
public:
  point3<T> m_origin;
  vec3<T> m_direction;

  ray3(){}
  ray3(point3<T> origin, vec3<T> direction) {
    m_origin = origin;
    m_direction = direction;
    m_direction.to_unit();
  }

  point3<T> origin() const { return m_origin; }
  vec3<T> direction() const { return m_direction; }

  point3<T> at(T param) const {
    return m_origin + param * m_direction;
  }
};

template <typename T>
inline std::ostream& operator<<(std::ostream &out, const ray3<T> &v) {
    return out << "o:" << v.origin() << ", dir:" << v.direction();
}

#endif
