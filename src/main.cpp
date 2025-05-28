// STL
#include <chrono>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>

// glm
#include "glm/vec3.hpp"

// src
#include "Scene.h"
#include "Ray.h"
#include "IRayTraceable.h"
#include "Sphere.h"

glm::vec3 ComputeRayColor(const Scene& scene, const Ray& ray) {
  std::optional<TraceResult> trace_result{
    scene.TraceRay(ray, 0.0F, std::numeric_limits<float>::infinity())
  };

  if (trace_result.has_value()) {
    return 0.5F * (trace_result.value().impact_normal + glm::vec3{ 1.0F, 1.0F, 1.0F });
  }

  const float a{
    0.5F * (ray.direction().y + 1.0F)
  };

  return (1.0F - a) * glm::vec3{ 1.0F, 1.0F, 1.0F }
         + a * glm::vec3{ 0.5F, 0.7F, 1.0F };
}

int main(int argc, char* argv[]) {
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

    // Render image
    for (std::size_t u{ 0U }; u < image_width; ++u) {
      // Compute current pixel position
      const glm::vec3 current_pixel_position{
        upper_left_pixel_position
        + static_cast<float>(u) * delta_u
        + static_cast<float>(v) * delta_v
      };

      // Compute ray direction and construct a ray with it
      const glm::vec3 ray_direction{ current_pixel_position - camera_position };
      const Ray ray{ camera_position, ray_direction };

      // Compute ray color by testing it against
      // all objects in the scene
      const glm::vec3 ray_color{ ComputeRayColor(scene, ray) };

      // Write ray color to output image file
      const int r{ static_cast<int>(255.999F * ray_color.r) };
      const int g{ static_cast<int>(255.999F * ray_color.g) };
      const int b{ static_cast<int>(255.999F * ray_color.b) };

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
