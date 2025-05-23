#pragma once

// GLM
#include "glm/vec3.hpp"

// Forward declarations
class Ray;

struct TraceResult
{
    glm::vec3 impact_point;
    glm::vec3 impact_normal;
    float distance;
    bool is_normal_outward;
};

class IRayTraceable
{
public:
    virtual ~IRayTraceable() = default;

    virtual bool TraceRay(
        TraceResult& trace_result,
        const Ray& ray,
        float min_distance,
        float max_distance
    ) const = 0;
};
