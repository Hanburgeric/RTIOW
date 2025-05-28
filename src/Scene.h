#ifndef SCENE_H
#define SCENE_H

// STL
#include <memory>
#include <optional>
#include <vector>

// Forward declarations
class Ray;
class IRayTraceable;
struct TraceResult;

class Scene {
public:
  Scene() = default;

  void AddObject(const std::shared_ptr<IRayTraceable>& object);

  [[nodiscard]]
  std::optional<TraceResult> TraceRay(
    const Ray& ray,
    float min_distance,
    float max_distance) const;

private:
  std::vector<std::shared_ptr<IRayTraceable>> ray_traceables_;
};

#endif
