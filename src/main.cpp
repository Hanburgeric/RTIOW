// STL
#include <iostream>
#include <fstream>

// GLM
#include "glm/glm.hpp"

// Core
#include "Ray.h"

glm::vec3 ComputePixelColor(const Ray& ray)
{
    const float a{ 0.5F * (ray.GetDirection().y + 1.0F) };
    return glm::vec3{
        (1.0F - a) * glm::vec3{ 1.0F } + a * glm::vec3{ 0.5F, 0.7F, 1.0F }
    };
}

int main()
{
    // Image settings
    constexpr float aspect_ratio{ 16.0F / 9.0F };
    constexpr std::size_t image_width{ 1280U };
    constexpr std::size_t image_height{
        static_cast<std::size_t>(static_cast<float>(image_width) / aspect_ratio)
    };

    // Camera settings
    constexpr glm::vec3 camera_position{ 0.0F };
    constexpr float focal_length{ 1.0F };

    // Viewport settings
    constexpr float viewport_height{ 2.0F };
    constexpr float viewport_width{
        viewport_height * (static_cast<float>(image_width) / static_cast<float>(image_height))
    };;

    constexpr glm::vec3 viewport_v{ 0.0F, -viewport_height, 0.0F };
    constexpr glm::vec3 viewport_u{ viewport_width, 0.0F, 0.0F };

    constexpr glm::vec3 delta_v{ viewport_v / static_cast<float>(image_height) };
    constexpr glm::vec3 delta_u{ viewport_u / static_cast<float>(image_width) };

    constexpr glm::vec3 viewport_upper_left_position{
        camera_position - glm::vec3{ 0.0F, 0.0F, focal_length }
        - 0.5F * (viewport_u + viewport_v)
    };
    constexpr glm::vec3 upper_left_pixel_position{
        viewport_upper_left_position
        + 0.5F * (delta_u + delta_v)
    };

    // Open output image file
    std::fstream output_image_file{ "image.ppm" };
    if (!output_image_file)
    {
        std::cerr << "Failed to open output image file.\n";
        return 1;
    }

    // Write to output image file
    output_image_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (std::size_t v{ 0U }; v < image_height; ++v) {
        std::clog << "\rScanlines remaining: " << (image_height - v) << ' ' << std::flush;
        for (std::size_t u{ 0U }; u < image_width; ++u) {
            const glm::vec3 pixel_position{
                upper_left_pixel_position
                + static_cast<float>(u) * delta_u
                + static_cast<float>(v) * delta_v
            };

            const glm::vec3 ray_direction{ pixel_position - camera_position };

            const Ray ray{
                Ray::FromOriginAndDirection(camera_position, ray_direction)
            };

            const glm::vec3 pixel_color{ ComputePixelColor(ray) };

            const int r{ static_cast<int>(255.999F * pixel_color.r) };
            const int g{ static_cast<int>(255.999F * pixel_color.g) };
            const int b{ static_cast<int>(255.999F * pixel_color.b) };

            output_image_file << r << ' ' << g << ' ' << b << '\n';
        }
    }
    std::clog << "\rDone.                 \n";

    // Close output image file
    output_image_file.close();

    return 0;
}
