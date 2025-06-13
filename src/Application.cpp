#include "Application.h"

// STL
#include <memory>
#include <string>

// spdlog
#include "spdlog/spdlog.h"

// SDL
#include "SDL3/SDL.h"

// imgui
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_opengl3_loader.h"

Application::Application()
    : platform_initialized_{ false }
    , window_{ nullptr, nullptr }
    , renderer_context_{ nullptr, nullptr }
    , gui_context_{ nullptr, nullptr }
    , gui_platform_initialized_{ nullptr }
    , gui_renderer_initialized_{ nullptr }
    , should_quit_{ false } {}

Application::~Application() {
  // Shutdown automatically just in case
  Shutdown();
}

bool Application::Initialize(SDL_InitFlags init_flags,
                             const std::string& window_title,
                             int window_width, int window_height,
                             SDL_WindowFlags window_flags) {
  // Initialize platform
  platform_initialized_ = SDL_Init(init_flags);
  if (!platform_initialized_) {
    spdlog::error(SDL_GetError());
    spdlog::error("Application platform failed to initialize.");
    return false;
  } else {
    spdlog::info("Application platform initialized.");
  }

  // Create window
  window_ = std::unique_ptr<SDL_Window,
                            decltype(&SDL_DestroyWindow)>{
      SDL_CreateWindow(window_title.c_str(),
                         window_width, window_height,
                         window_flags),
      SDL_DestroyWindow
  };
  if (!window_) {
    spdlog::error(SDL_GetError());
    spdlog::error("Application failed to create window.");
    return false;
  } else {
    spdlog::info("Application window created.");
  }

  // Create renderer context
  renderer_context_ = std::unique_ptr<SDL_GLContextState,
                                      decltype(&SDL_GL_DestroyContext)>{
    SDL_GL_CreateContext(window_.get()),
    SDL_GL_DestroyContext
  };
  if (!renderer_context_) {
    spdlog::error(SDL_GetError());
    spdlog::error("Application failed to create renderer context.");
    return false;
  } else {
    spdlog::info("Application renderer context created.");
  }

  // Create GUI context
  gui_context_ = std::unique_ptr<ImGuiContext,
                                 decltype(&ImGui::DestroyContext)>{
   ImGui::CreateContext(),
   ImGui::DestroyContext
  };
  if (!gui_context_) {
    spdlog::error("Application failed to create GUI context.");
    return false;
  } else {
    spdlog::info("Application GUI context created.");
  }

  // Configure GUI context
  gui_context_->IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  gui_context_->IO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  gui_context_->IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  gui_context_->IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

  // Initialize GUI for platform
  gui_platform_initialized_ = ImGui_ImplSDL3_InitForOpenGL(
    window_.get(), renderer_context_.get()
  );
  if (!gui_platform_initialized_) {
    spdlog::info("Application GUI failed to initialize for platform.");
    return false;
  } else {
    spdlog::info("Application GUI initialized for platform.");
  }

  // Initialize GUI for renderer
  gui_renderer_initialized_ = ImGui_ImplOpenGL3_Init();
  if (!gui_renderer_initialized_) {
    spdlog::info("Application GUI failed to initialize for renderer.");
    return false;
  } else {
    spdlog::info("Application GUI initialized for renderer.");
  }

  return true;
}

void Application::Run() {
  // Main loop
  while (!should_quit_) {
    // Handle events
    SDL_Event event{};
    while (SDL_PollEvent(&event)) {
      // Allow GUI to handle event first
      ImGui_ImplSDL3_ProcessEvent(&event);

      switch (event.type) {
        case SDL_EVENT_QUIT: { OnQuit(); break; }
        case SDL_EVENT_WINDOW_RESIZED: { OnWindowResized(); break; }
        default: { break; }
      }
    }

    // Begin new GUI frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    // TEMPORARY
    ImGui::ShowDemoWindow();

    // Update physics
    // ???

    // Render
    // Clear buffer
    glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render GUI
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    SDL_GL_MakeCurrent(window_.get(), renderer_context_.get());

    // Swap buffers
    SDL_GL_SwapWindow(window_.get());
  }
}

void Application::Shutdown() {
  // Shutdown GUI for renderer
  if (gui_renderer_initialized_) {
    ImGui_ImplOpenGL3_Shutdown();
    gui_renderer_initialized_ = false;
    spdlog::info("Application GUI shutdown for renderer.");
  }

  // Shutdown GUI for platform
  if (gui_platform_initialized_) {
    ImGui_ImplSDL3_Shutdown();
    gui_platform_initialized_ = false;
    spdlog::info("Application GUI shutdown for platform.");
  }

  // Destroy GUI context
  if (gui_context_) {
    gui_context_.reset();
    spdlog::info("Application GUI context destroyed.");
  }

  // Destroy renderer context
  if (renderer_context_) {
    renderer_context_.reset();
    spdlog::info("Application renderer context destroyed.");
  }

  // Destroy window
  if (window_) {
    window_.reset();
    spdlog::info("Application window destroyed.");
  }

  // Shutdown platform
  if (platform_initialized_) {
    SDL_Quit();
    platform_initialized_ = false;
    spdlog::info("Application platform shut down.");
  }
}
void Application::OnQuit() {
  should_quit_ = true;
}

void Application::OnWindowResized() {
  int window_width{};
  int window_height{};
  SDL_GetWindowSize(window_.get(), &window_width, &window_height);
  glViewport(0, 0, window_width, window_height);
}
