#include "Image.h"

// STL
#include <stdexcept>

Image::Image(std::size_t image_width, std::size_t image_height)
    : image_width_{ image_width }
    , image_height_{ image_height }
    , image_data_{ image_width_ * image_height }
{
}

std::size_t Image::GetImageWidth() const
{
    return image_width_;
}

std::size_t Image::GetImageHeight() const
{
    return image_height_;
}

const std::vector<glm::vec4>& Image::GetImageData() const
{
    return image_data_;
}

const glm::vec4& Image::GetPixel(std::size_t x, std::size_t y) const
{
    return image_data_.at(GetPixelIndex(x, y));
}

void Image::SetPixel(std::size_t x, std::size_t y, const glm::vec4& pixel_color)
{
    image_data_.at(GetPixelIndex(x, y)) = pixel_color;
}

std::size_t Image::GetPixelIndex(std::size_t x, std::size_t y) const
{
    if (x >= image_width_ || y >= image_height_)
    {
        throw std::out_of_range("Pixel coordinate out of bounds");
    }

    return y * image_width_ + x;
}
