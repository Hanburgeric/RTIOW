#include "Sphere.h"

// STL
#include <cassert>
#include <optional>

// glm
#include "glm/vec3.hpp"
#include "glm/geometric.hpp"

// src
#include "Ray.h"
#include "IRayTraceable.h"

// Forward declarations
class Ray;

Sphere::Sphere(const glm::vec3& center, float radius)
    : center_{center}, radius_{radius} {
  assert(radius_ > 0.0F && "Sphere radius must be positive");
}

std::optional<TraceResult> Sphere::TraceRay(
    const Ray& ray,
    float min_distance,
    float max_distance) const {
  // Compute quadratic terms needed to determine the discriminant
  const glm::vec3 origin_to_center{ center_ - ray.origin() };
  const float h{
    glm::dot(origin_to_center, ray.direction())
  };
  const float c{
    glm::dot(origin_to_center, origin_to_center)
    - radius_ * radius_
  };

  // Compute discriminant, return if no roots exist
  // (i.e. no collision with ray)
  const float discriminant{ h * h - c };
  if (discriminant < 0.0F) {
    return std::nullopt;
  }

  // Find the closer of the two roots that also lies within the range
  // defined by the minimum and maximum distances
  const float sqrt_d{ glm::sqrt(discriminant) };

  // Try the negative root
  float root{ h - sqrt_d };
  if (root <= min_distance || max_distance <= root) {
    // Try the positive root
    root = (h + sqrt_d);
    if (root <= min_distance || max_distance <= root) {
      // Neither root is within range
      return std::nullopt;
    }
  }

  // Root is within range, record and return trace result
  TraceResult trace_result{};
  trace_result.distance = root;
  trace_result.impact_position = ray.At(trace_result.distance);
  trace_result.impact_normal = (trace_result.impact_position - center_) / radius_;
  trace_result.is_front_face =
    glm::dot(ray.direction(), trace_result.impact_normal) < 0.0F;

  // Flip the normal if the collision occurred from within the sphere
  if (!trace_result.is_front_face) {
    trace_result.impact_normal = -trace_result.impact_normal;
  }

  return trace_result;
}
