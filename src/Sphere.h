#ifndef SPHERE_H
#define SPHERE_H

// STL
#include <optional>

// glm
#include "glm/vec3.hpp"

// src
#include "IRayTraceable.h"

// Forward declarations
class Ray;

class Sphere final : public IRayTraceable {
public:
  Sphere() = delete;
  Sphere(const glm::vec3& center, float radius);

  [[nodiscard]]
  constexpr const glm::vec3& center() const noexcept {
    return center_;
  }

  [[nodiscard]]
  constexpr float radius() const noexcept {
    return radius_;
  }

  [[nodiscard]]
  std::optional<TraceResult> TraceRay(
    const Ray& ray,
    float min_distance,
    float max_distance) const override;

private:
  glm::vec3 center_;
  float radius_;
};

#endif
