#pragma once

// STL
#include <cstddef>
#include <vector>

// GLM
#include "glm/vec4.hpp"

class Image
{
public:
    Image() = default;
    Image(std::size_t image_width, std::size_t image_height);

    [[nodiscard]]
    std::size_t GetImageWidth() const;

    [[nodiscard]]
    std::size_t GetImageHeight() const;

    [[nodiscard]]
    const std::vector<glm::vec4>& GetImageData() const;

    [[nodiscard]]
    const glm::vec4& GetPixel(std::size_t x, std::size_t y) const;

    void SetPixel(std::size_t x, std::size_t y, const glm::vec4& pixel_color);

private:
    [[nodiscard]]
    std::size_t GetPixelIndex(std::size_t x, std::size_t y) const;

private:
    std::size_t image_width_;
    std::size_t image_height_;
    std::vector<glm::vec4> image_data_;
};
