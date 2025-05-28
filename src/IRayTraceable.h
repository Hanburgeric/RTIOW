#ifndef IRAYTRACEABLE_H
#define IRAYTRACEABLE_H

// STL
#include <optional>

// glm
#include "glm/vec3.hpp"

// Forward declarations
class Ray;

struct TraceResult {
  glm::vec3 impact_position;
  glm::vec3 impact_normal;
  float distance;
  bool is_front_face;
};

class IRayTraceable {
public:
  virtual ~IRayTraceable() = default;

  [[nodiscard]]
  virtual std::optional<TraceResult> TraceRay(
    const Ray& ray,
    float min_distance,
    float max_distance) const = 0;
};

#endif
