#pragma once

// GLM
#include "glm/vec3.hpp"

class Ray
{
public:
    Ray() = default;
    Ray(const glm::vec3& origin, const glm::vec3& direction);

    [[nodiscard]]
    const glm::vec3& GetOrigin() const;

    [[nodiscard]]
    const glm::vec3& GetDirection() const;

    [[nodiscard]]
    glm::vec3 At(float t) const;

private:
    glm::vec3 origin_;
    glm::vec3 direction_;
};
