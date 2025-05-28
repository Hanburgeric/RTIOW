#ifndef RAY_H
#define RAY_H

// glm
#include "glm/vec3.hpp"

class Ray {
public:
  Ray() = delete;
  Ray(const glm::vec3& origin, const glm::vec3& direction);

  [[nodiscard]]
  constexpr const glm::vec3& origin() const noexcept {
    return origin_;
  }

  [[nodiscard]]
  constexpr const glm::vec3& direction() const noexcept {
    return direction_;
  }

  [[nodiscard]]
  constexpr glm::vec3 At(float distance) const noexcept {
    return origin_ + distance * direction_;
  }

private:
  glm::vec3 origin_;
  glm::vec3 direction_;
};

#endif
