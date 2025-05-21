#pragma once

// GLM
#include "glm/vec3.hpp"

class Ray
{
public:
    Ray() = delete;

    static Ray FromOriginAndDirection(
        const glm::vec3& origin,
        const glm::vec3& direction
    );

    static Ray FromOriginAndUnitDirection(
        const glm::vec3& origin,
        const glm::vec3& unit_direction
    );

    [[nodiscard]]
    constexpr const glm::vec3& GetOrigin() const noexcept { return origin_; }

    [[nodiscard]]
    constexpr const glm::vec3& GetDirection() const noexcept { return direction_; }

    [[nodiscard]]
    constexpr glm::vec3 At(float distance) const noexcept{ return origin_ + distance * direction_; }

private:
    Ray(const glm::vec3& origin, const glm::vec3& direction);

private:
    glm::vec3 origin_;
    glm::vec3 direction_;
};
