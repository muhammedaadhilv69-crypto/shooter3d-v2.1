#include "Window.h"
#include <iostream>

namespace Engine {

Window::Window(const std::string& title, int width, int height)
    : m_window(nullptr)
    , m_context(nullptr)
    , m_title(title)
    , m_width(width)
    , m_height(height)
    , m_running(false)
    , m_deltaTime(0.0f)
    , m_lastTime(0)
    , m_mouseCaptured(false)
{
}

Window::~Window() {
    shutdown();
}

bool Window::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return false;
    }

    // Request OpenGL 3.3 Core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    m_window = SDL_CreateWindow(
        m_title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        m_width,
        m_height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    if (!m_window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        return false;
    }

    m_context = SDL_GL_CreateContext(m_window);
    if (!m_context) {
        std::cerr << "OpenGL context creation failed: " << SDL_GetError() << std::endl;
        return false;
    }

    // Load all OpenGL 3.3 function pointers via glad
    if (!gladLoadGL()) {
        std::cerr << "Failed to load OpenGL functions (glad)!" << std::endl;
        return false;
    }

    SDL_GL_SetSwapInterval(1); // VSync

    // Now safe to call modern GL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    m_running = true;
    m_lastTime = SDL_GetTicks();

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    return true;
}

void Window::processEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                m_running = false;
                break;

            case SDL_KEYDOWN:
            case SDL_KEYUP:
                if (m_keyCallback) {
                    m_keyCallback(event.key.keysym.scancode, event.type == SDL_KEYDOWN);
                }
                if (event.key.keysym.sym == SDLK_ESCAPE && event.type == SDL_KEYDOWN) {
                    m_running = false;
                }
                break;

            case SDL_MOUSEMOTION:
                if (m_mouseCaptured && m_mouseCallback) {
                    m_mouseCallback(event.motion.xrel, event.motion.yrel);
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                if (m_keyCallback && event.button.button == SDL_BUTTON_LEFT) {
                    m_keyCallback(1000, event.type == SDL_MOUSEBUTTONDOWN);
                }
                break;
        }
    }

    // Update delta time
    Uint32 currentTime = SDL_GetTicks();
    m_deltaTime = (currentTime - m_lastTime) / 1000.0f;
    m_lastTime = currentTime;
}

void Window::swap() {
    SDL_GL_SwapWindow(m_window);
}

void Window::captureMouse(bool capture) {
    m_mouseCaptured = capture;
    SDL_SetRelativeMouseMode(capture ? SDL_TRUE : SDL_FALSE);
    SDL_ShowCursor(capture ? SDL_DISABLE : SDL_ENABLE);
}

void Window::shutdown() {
    if (m_context) {
        SDL_GL_DeleteContext(m_context);
        m_context = nullptr;
    }
    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
    m_running = false;
    SDL_Quit();
}

} // namespace Engine
