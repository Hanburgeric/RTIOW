cmake_minimum_required(VERSION 3.25)
project(RTIOW)

# ======================================================================
# Compiler Settings
# ======================================================================
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<CONFIG>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")

# ======================================================================
# Dependencies
# ======================================================================
find_package(OpenGL REQUIRED)

include(FetchContent)

FetchContent_Declare(
    glm             # 1.0.1
    GIT_REPOSITORY  https://github.com/g-truc/glm.git
    GIT_TAG         0af55ccecd98d4e5a8d1fad7de25ba429d60e863
)

FetchContent_Declare(
    imgui           # v1.91.9b-docking
    GIT_REPOSITORY  https://github.com/ocornut/imgui.git
    GIT_TAG         4806a1924ff6181180bf5e4b8b79ab4394118875
)

FetchContent_Declare(
    SDL3            # release-3.2.14
    GIT_REPOSITORY  https://github.com/libsdl-org/SDL.git
    GIT_TAG         8d604353a53853fa56d1bdce0363535605ca868f
)

FetchContent_Declare(
    spdlog          # v1.15.3
    GIT_REPOSITORY  https://github.com/gabime/spdlog.git
    GIT_TAG         6fa36017cfd5731d617e1a934f0e5ea9c4445b13
)

FetchContent_MakeAvailable(
    glm
    imgui
    SDL3
    spdlog
)

# ======================================================================
# Main Executable
# ======================================================================
add_executable(
    ${PROJECT_NAME}
    src/Application.cpp src/Application.h
    src/IRayTraceable.h
    src/main.cpp
    src/Ray.cpp src/Ray.h
    src/Scene.cpp src/Scene.h
    src/Sphere.cpp src/Sphere.h
)

target_sources(
    ${PROJECT_NAME} PRIVATE
        ${imgui_SOURCE_DIR}/imconfig.h
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui.h
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_internal.h
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imstb_rectpack.h
        ${imgui_SOURCE_DIR}/imstb_textedit.h
        ${imgui_SOURCE_DIR}/imstb_truetype.h

        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3_loader.h
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.h

        ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
        ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.h
)

target_include_directories(
    ${PROJECT_NAME} PRIVATE
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
        glm::glm
        OpenGL::GL
        SDL3::SDL3
        spdlog::spdlog
)

# ======================================================================
# Post Build Commands
# ======================================================================
get_filename_component(
    CMAKE_CXX_COMPILER_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY
)

foreach(
    dll IN ITEMS
        libstdc++-6.dll
        libgcc_s_seh-1.dll
        libwinpthread-1.dll
)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CXX_COMPILER_DIR}/${dll}"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
endforeach()
