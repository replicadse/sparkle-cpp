#ifndef CAMERA_H
#define CAMERA_H

class camera {
private:
  point3<float> m_origin, m_lower_left_corner;
  vec3<float> m_horizontal, m_vertical, m_u, m_v, m_w;
  float m_lens_radius;

public:
  camera(
    point3<float> origin, vec3<float> direction,
    point3<float> v_up, float v_fov,
    float aspect_ratio, float aperture, float focus_dist) {
    auto h = tan(v_fov/2);
    auto vp_height = h * 2;
    auto vp_width = aspect_ratio * vp_height;

    auto m_w = direction.unit();
    auto m_u = v_up.cross(m_w).unit();
    auto m_v = m_w.cross(m_u);

    m_origin = origin;
    m_horizontal = focus_dist * vp_width * m_u;
    m_vertical = focus_dist * vp_height * m_v;
    m_lower_left_corner = m_origin - m_horizontal/2 - m_vertical/2 - focus_dist * m_w;
    m_lens_radius = aperture / 2;
  }

  ray3<float> shoot(float s, float t) const {
    auto rd = m_lens_radius * random_in_unit_disk<float>();
    auto offset = m_u * rd.x() + m_v * rd.y();
    return ray3<float>(
      m_origin + offset,
      m_lower_left_corner + s*m_horizontal + t*m_vertical - m_origin - offset);
  }
};

#endif
