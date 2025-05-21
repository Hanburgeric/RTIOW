#include "Ray.h"

// GLM
#include "glm/geometric.hpp"

Ray Ray::FromOriginAndDirection(
    const glm::vec3& origin,
    const glm::vec3& direction
)
{
    return Ray{ origin, glm::normalize(direction) };
}

Ray Ray::FromOriginAndUnitDirection(
    const glm::vec3& origin,
    const glm::vec3& unit_direction
)
{
    return Ray{ origin, unit_direction };
}

Ray::Ray(const glm::vec3& origin, const glm::vec3& direction)
    : origin_{ origin }
    , direction_ { direction }
{
    assert(
        glm::abs(glm::length(direction_) - 1.0F) < 1e-6F
        && "Ray direction must have a length of 1."
    );
}
