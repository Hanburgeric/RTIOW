cmake_minimum_required(VERSION 3.25)
project(RTIOW)

# ======================================================================
# Compiler Settings
# ======================================================================
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ======================================================================
# Dependencies
# ======================================================================
include(FetchContent)

FetchContent_Declare(
    glm             # 1.0.1
    GIT_REPOSITORY  https://github.com/g-truc/glm.git
    GIT_TAG         0af55ccecd98d4e5a8d1fad7de25ba429d60e863
)

FetchContent_MakeAvailable(
    glm
)

# ======================================================================
# Main Executable
# ======================================================================
add_executable(
    ${PROJECT_NAME}
    src/main.cpp

    src/Core/Ray.cpp src/Core/Ray.h

    src/Image/IImageWriter.h
    src/Image/Image.cpp src/Image/Image.h
    src/Image/PpmWriter.cpp src/Image/PpmWriter.h
)

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    glm::glm
)
