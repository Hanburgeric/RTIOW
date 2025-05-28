#include "Scene.h"

// STL
#include <memory>
#include <optional>
#include <vector>

// src
#include "IRayTraceable.h"

void Scene::AddObject(const std::shared_ptr<IRayTraceable>& object) {
  ray_traceables_.emplace_back(object);
}

std::optional<TraceResult> Scene::TraceRay(
    const Ray& ray,
    float min_distance,
    float max_distance) const {
  TraceResult trace_result{};

  // Test ray intersection with every ray traceable object
  bool hit_anything{ false };
  for (const std::shared_ptr<IRayTraceable>& ray_traceable : ray_traceables_) {
    const std::optional<TraceResult> local_trace_result{
      ray_traceable->TraceRay(ray, min_distance, max_distance)
    };

    // Constantly store the final result with the nearest distance
    // by updating the maximum trace distance
    if (local_trace_result.has_value()) {
      hit_anything = true;
      trace_result = local_trace_result.value();
      max_distance = trace_result.distance;
    }
  }

  // Ray did not collide with anything
  if (!hit_anything) {
    return std::nullopt;
  }

  return trace_result;
}
