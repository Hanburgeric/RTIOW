// STL
#include <chrono>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <random>

// glm
#include "glm/common.hpp"
#include "glm/vec3.hpp"

// src
#include "Scene.h"
#include "Ray.h"
#include "IRayTraceable.h"
#include "Sphere.h"

glm::vec3 ComputeRayColor(const Scene& scene, const Ray& ray) {
  // Trace ray against all traceable objects in the scene
  std::optional<TraceResult> trace_result{
    scene.TraceRay(ray, 0.0F, std::numeric_limits<float>::infinity())
  };

  // If anything was hit, color the ray with its impact normal
  if (trace_result.has_value()) {
    return 0.5F * (trace_result.value().impact_normal + glm::vec3{ 1.0F, 1.0F, 1.0F });
  }

  // Otherwise, return a background color gradient
  const float a{
    0.5F * (ray.direction().y + 1.0F)
  };

  return (1.0F - a) * glm::vec3{ 1.0F, 1.0F, 1.0F }
         + a * glm::vec3{ 0.5F, 0.7F, 1.0F };
}

int main(int argc, char* argv[]) {
  // Create random number generator for antialiasing
  std::random_device rd{};
  std::mt19937 gen{ rd() };
  std::uniform_real_distribution dist{ -0.5F, 0.5F };

  // Ray tracing settings
  constexpr size_t samples_per_pixel{ 100 };
  constexpr float weight_per_sample{ 1.0F / static_cast<float>(samples_per_pixel) };

  // Scene settings
  Scene scene{};
  scene.AddObject(std::make_shared<Sphere>(
    glm::vec3{ 0.0F, 0.0F, -1.0F}, 0.5F)
  );
  scene.AddObject(std::make_shared<Sphere>(
    glm::vec3{ 0.0F, -100.5F, -1.0F}, 100.0F)
  );

  // Camera settings
  constexpr glm::vec3 camera_position{ 0.0F, 0.0F, 0.0F };
  constexpr float focal_length{ 1.0F };

  // Image settings
  constexpr float aspect_ratio{ 16.0F / 9.0F };

  constexpr size_t image_width{ 1280U };
  constexpr size_t image_height{
    static_cast<size_t>(static_cast<float>(image_width) / aspect_ratio)
  };

  constexpr float viewport_height{ 2.0F };
  constexpr float viewport_width{
    viewport_height * (static_cast<float>(image_width) / static_cast<float>(image_height))
  };

  constexpr glm::vec3 viewport_v{ 0.0F, -viewport_height, 0.0F };
  constexpr glm::vec3 viewport_u{ viewport_width, 0.0F, 0.0F };

  constexpr glm::vec3 delta_v{ viewport_v / static_cast<float>(image_height) };
  constexpr glm::vec3 delta_u{ viewport_u / static_cast<float>(image_width) };

  constexpr glm::vec3 viewport_upper_left_position {
    camera_position
    - glm::vec3{ 0.0F, 0.0F, focal_length }
    - 0.5F * (viewport_u + viewport_v)
  };

  constexpr glm::vec3 upper_left_pixel_position {
    viewport_upper_left_position
    + 0.5F * (delta_u + delta_v)
  };

  // Open output image file
  std::ofstream output_image_file{ "image.ppm" };
  if (!output_image_file) {
    std::cerr << "Failed to open output image file." << std::endl;
    return 1;
  }

  // Begin timer
  const std::chrono::time_point start_time{ std::chrono::high_resolution_clock::now() };

  // Write header to output image file
  output_image_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  // Render image by iterating over every pixel
  for (size_t v{ 0U }; v < image_height; ++v) {
    // Log progress
    std::clog << "\rScanlines remaining: " << image_height - v << ' ' << std::flush;

    for (size_t u{ 0U }; u < image_width; ++u) {
      // Zero-initialize eventual color for the pixel
      glm::vec3 pixel_color{};

      // Accumulate color by iterating over subpixel samples
      for (size_t sample{ 0U }; sample < samples_per_pixel; ++sample) {
        // Generate a random half-pixel offset for sample
        glm::vec3 sample_offset{ dist(gen), dist(gen), 0.0F };

        // Compute current sample position
        const glm::vec3 sample_position{
          upper_left_pixel_position
          + (static_cast<float>(u) + sample_offset.x) * delta_u
          + (static_cast<float>(v) + sample_offset.y) * delta_v
        };

        // Compute ray origin and direction for sample
        const glm::vec3 sample_ray_origin{ camera_position };
        const glm::vec3 sample_ray_direction{ sample_position - sample_ray_origin };

        // Construct ray for sample
        const Ray sample_ray{ sample_ray_origin, sample_ray_direction };

        // Compute color for ray
        const glm::vec3 sample_ray_color{ ComputeRayColor(scene, sample_ray) };

        // Accumulate color
        pixel_color += sample_ray_color * weight_per_sample;
      }

      // Clamp and write pixel color to output image file
      const int r{ static_cast<int>(255.999F * glm::clamp(pixel_color.r, 0.0F, 0.999F)) };
      const int g{ static_cast<int>(255.999F * glm::clamp(pixel_color.g, 0.0F, 0.999F)) };
      const int b{ static_cast<int>(255.999F * glm::clamp(pixel_color.b, 0.0F, 0.999F)) };

      output_image_file << r << ' ' << g << ' ' << b << '\n';
    }
  }
  std::clog << "\nRender complete." << std::endl;

  // Stop timer
  const std::chrono::time_point end_time{ std::chrono::high_resolution_clock::now() };

  // Compute and log elapsed time
  const std::chrono::duration<float> elapsed_time{ end_time - start_time };
  std::cout << "Image rendered in " << elapsed_time.count() << " seconds." << std::endl;

  // Close output image file
  output_image_file.close();

  return 0;
}
