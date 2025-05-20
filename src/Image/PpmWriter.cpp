#include "PpmWriter.h"

// STL
#include <algorithm>
#include <fstream>
#include <iostream>

bool PpmWriter::WriteImage(
    const std::string& file_name,
    std::size_t image_width,
    std::size_t image_height,
    const std::vector<glm::vec4>& image_data
) const
{
    if (image_data.size() != image_width * image_height) {
        return false; // Mismatch in data size
    }

    std::ofstream out_file(file_name);
    if (!out_file.is_open()) {
        return false; // Failed to open file
    }

    out_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (std::size_t i = 0; i < image_data.size(); ++i) {
        const glm::vec4& color = image_data[i];

        // Clamp and convert
        auto to_int = [](float c) {
            return static_cast<int>(255.999f * std::clamp(c, 0.0f, 1.0f));
        };

        int r = to_int(color.r);
        int g = to_int(color.g);
        int b = to_int(color.b); // Ignore alpha for PPM

        out_file << r << ' ' << g << ' ' << b << '\n';
    }

    return true;
}
