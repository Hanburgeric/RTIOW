#include "Ray.h"

Ray::Ray(const glm::vec3& origin, const glm::vec3& direction)
    : origin_{ origin }
    , direction_ { direction }
{
}

const glm::vec3& Ray::GetOrigin() const
{
    return origin_;
}

const glm::vec3& Ray::GetDirection() const
{
    return direction_;
}

glm::vec3 Ray::At(float t) const
{
    return origin_ + t * direction_;
}
