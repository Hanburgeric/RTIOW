#ifndef APPLICATION_H
#define APPLICATION_H

// STL
#include <memory>
#include <string>

// SDL
#include "SDL3/SDL.h"

// imgui
#include "imgui.h"

class Application {
public:
  Application();
  ~Application();

  bool Initialize(const std::string& window_title,
                  int window_width, int window_height);
  void Run();
  void Shutdown();

private:
  void OnQuit();
  void OnWindowResized();

private:
  bool platform_initialized_;
  std::unique_ptr<SDL_Window,
                  decltype(&SDL_DestroyWindow)> window_;
  std::unique_ptr<SDL_GLContextState,
                  decltype(&SDL_GL_DestroyContext)> renderer_context_;
  std::unique_ptr<ImGuiContext,
                  decltype(&ImGui::DestroyContext)> gui_context_;
  bool gui_platform_initialized_;
  bool gui_renderer_initialized_;
  bool should_quit_;
};

#endif  // APPLICATION_H
