#include "Ray.h"

// STL
#include <cassert>

// glm
#include "glm/vec3.hpp"
#include "glm/geometric.hpp"

Ray::Ray(const glm::vec3& origin, const glm::vec3& direction)
    : origin_{ origin }
, direction_{ glm::normalize(direction) } {
  assert(glm::abs(glm::length(direction_) - 1.0F) < 1e-6F
         && "Ray direction must be a unit vector");
}
