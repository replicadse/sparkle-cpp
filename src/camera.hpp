#ifndef CAMERA_H
#define CAMERA_H

class camera {
private:
  point3<float> m_origin;
  point3<float> m_lower_left_corner;
  vec3<float> m_horizontal;
  vec3<float> m_vertical;

public:
  camera(const float w, const float h, const float fl) {
    m_origin = point3<float>(0, 0, 0);
    m_horizontal = vec3<float>(w, 0, 0);
    m_vertical = vec3<float>(0, h, 0);
    m_lower_left_corner = m_origin - m_horizontal/2 - m_vertical/2 - vec3<float>(0, 0, fl);
  }

  ray3<float> shoot(float u, float v) const {
    return ray3<float>(m_origin, m_lower_left_corner + u*m_horizontal + v*m_vertical - m_origin);
  }
};

#endif
