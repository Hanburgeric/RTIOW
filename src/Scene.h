#pragma once

// STL
#include <memory>
#include <vector>

// Forward declarations
struct TraceResult;
class IRayTraceable;
class Ray;

class Scene
{
public:
    Scene() = default;

    void AddObject(const std::shared_ptr<IRayTraceable>& object);

    bool TraceRay(
        TraceResult& trace_result,
        const Ray& ray,
        float min_distance,
        float max_distance
    ) const;

private:
    std::vector<std::shared_ptr<IRayTraceable>> ray_traceable_objects_;
};
