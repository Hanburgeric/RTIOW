#include "Sphere.h"

// GLM
#include "glm/geometric.hpp"

// src
#include "Ray.h"

Sphere::Sphere(const glm::vec3& center, float radius)
    : center_(center)
    , radius_(radius)
{
    assert(radius > 0.0F && "Sphere radius must be greater than 0.");
}

bool Sphere::TraceRay(
    TraceResult& trace_result,
    const Ray& ray,
    float min_distance,
    float max_distance
) const
{
    const glm::vec3 OriginToCenter{ center_ - ray.GetOrigin() };
    const float h{ glm::dot(ray.GetDirection(), OriginToCenter) };
    const float c{
        glm::dot(OriginToCenter, OriginToCenter) - radius_ * radius_
    };

    const float discriminant{ h * h - c };
    if (discriminant < 0.0F)
    {
        return false;
    }

    const float sqrt_d{ glm::sqrt(discriminant) };

    float root{ h - sqrt_d };
    if (root <= min_distance || max_distance <= root)
    {
        root = h + sqrt_d;
        if (root <= min_distance || max_distance <= root)
        {
            return false;
        }
    }

    trace_result.distance = root;
    trace_result.impact_point = ray.At(trace_result.distance);
    trace_result.impact_normal = (trace_result.impact_point - center_) / radius_;
    trace_result.is_normal_outward =
        glm::dot(ray.GetDirection(), trace_result.impact_normal) < 0.0F;

    return true;
}
