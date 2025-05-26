#pragma once

// GLM
#include "glm/vec3.hpp"

// src
#include "IRayTraceable.h"

class Sphere final : public IRayTraceable
{
public:
    Sphere() = delete;

    Sphere(const glm::vec3& center, float radius);

    [[nodiscard]]
    constexpr const glm::vec3& GetCenter() const noexcept { return center_; }

    [[nodiscard]]
    constexpr float GetRadius() const noexcept { return radius_; }

    bool TraceRay(
        TraceResult& trace_result,
        const Ray& ray,
        float min_distance,
        float max_distance
    ) const override;

private:
    glm::vec3 center_;
    float radius_;
};
