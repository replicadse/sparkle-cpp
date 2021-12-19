#ifndef GEOMETRY_GROUP_H
#define GEOMETRY_GROUP_H

template <typename T>
class geometry_group : public geometry<T> {
public:
  vector<shared_ptr<geometry<T>>> m_objects;

  geometry_group() {}

  void clear() {
    m_objects.clear();
  }
  void add(shared_ptr<geometry<T>> object) {
    m_objects.push_back(object);
  }

  bool hit(const ray3<T>& r, T t_min, T t_max, hit_rec<T>& rec) const {
    hit_rec<T> temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : m_objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
  }
};

#endif
