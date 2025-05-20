#pragma once

// src
#include "IImageWriter.h"

class PpmWriter : public IImageWriter
{
public:
    [[nodiscard]]
    bool WriteImage(
        const std::string& file_name,
        std::size_t image_width,
        std::size_t image_height,
        const std::vector<glm::vec4>& image_data
    ) const final;
};
