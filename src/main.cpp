// STL
#include <iostream>
#include <vector>

// GLM
#include "glm/glm.hpp"

// Core
#include "Core/Ray.h"

// Image I/O
#include "Image/Image.h"
#include "Image/PpmWriter.h"

glm::vec4 ComputePixelColor(const Ray& ray)
{
    const glm::vec3 unit_direction = glm::normalize(ray.GetDirection());
    const float a = 0.5f * (unit_direction.y + 1.0f);
    const glm::vec3 rgb = (1.0f - a) * glm::vec3{1.0f} + a * glm::vec3{0.5f, 0.7f, 1.0f};
    return glm::vec4(rgb, 1.0f); // opaque
}

int main()
{
    // Image settings
    const float aspect_ratio = 16.0f / 9.0f;
    const std::size_t image_width = 1280;
    const std::size_t image_height = static_cast<std::size_t>(image_width / aspect_ratio);
    const std::size_t pixel_count = image_width * image_height;

    // Camera + Viewport
    const glm::vec3 camera_position{0.0f};
    const float focal_length = 1.0f;
    const float viewport_height = 2.0f;
    const float viewport_width = viewport_height * (static_cast<float>(image_width) / image_height);

    const glm::vec3 viewport_u{viewport_width, 0.0f, 0.0f};
    const glm::vec3 viewport_v{0.0f, -viewport_height, 0.0f};
    const glm::vec3 delta_u = viewport_u / static_cast<float>(image_width);
    const glm::vec3 delta_v = viewport_v / static_cast<float>(image_height);
    const glm::vec3 first_pixel_position =
            camera_position - glm::vec3(0.0f, 0.0f, focal_length)
            - viewport_u * 0.5f - viewport_v * 0.5f
            + 0.5f * (delta_u + delta_v);

    // Generate image
    Image image{ image_width, image_height };
    for (std::size_t y = 0; y < image_height; ++y) {
        std::clog << "\rScanlines remaining: " << (image_height - y) << ' ' << std::flush;
        for (std::size_t x = 0; x < image_width; ++x) {
            const glm::vec3 pixel_center = first_pixel_position
                                           + static_cast<float>(x) * delta_u
                                           + static_cast<float>(y) * delta_v;

            const Ray ray{camera_position, pixel_center - camera_position};
            const glm::vec4 color = ComputePixelColor(ray);
            image.SetPixel(x, y, color);
        }
    }
    std::clog << "\rDone.                 \n";

    // Write image to file
    PpmWriter writer;
    const std::string output_filename = "image.ppm";
    const bool success = writer.WriteImage(output_filename, image.GetImageWidth(), image.GetImageHeight(), image.GetImageData());

    if (!success) {
        std::cerr << "Failed to write image to file: " << output_filename << "\n";
        return 1;
    }

    return 0;
}
