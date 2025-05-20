#pragma once

// STL
#include <cstddef>
#include <string>
#include <vector>

// GLM
#include "glm/vec4.hpp"

class IImageWriter
{
public:
    virtual ~IImageWriter() = default;

    [[nodiscard]]
    virtual bool WriteImage(
        const std::string& file_name,
        std::size_t image_width,
        std::size_t image_height,
        const std::vector<glm::vec4>& image_data
    ) const = 0;
};
