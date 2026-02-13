#include "core/Window.hpp"
#include "core/Logger.hpp"
#include <GLFW/glfw3.h>

namespace fps::core {

Window::Window()
    : m_window(nullptr)
    , m_width(0)
    , m_height(0)
    , m_windowedWidth(0)
    , m_windowedHeight(0)
    , m_fullscreen(false)
    , m_focused(true) {
}

Window::~Window() {
    Shutdown();
}

bool Window::Initialize(const WindowConfig& config) {
    LOG_INFO("Initializing window...");
    
    if (!glfwInit()) {
        LOG_FATAL("Failed to initialize GLFW");
        return false;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config.openglMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config.openglMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, config.resizable ? GLFW_TRUE : GLFW_FALSE);
    
    if (config.msaaSamples > 0) {
        glfwWindowHint(GLFW_SAMPLES, config.msaaSamples);
    }
    
    m_width = config.width;
    m_height = config.height;
    m_windowedWidth = config.width;
    m_windowedHeight = config.height;
    m_title = config.title;
    m_fullscreen = config.fullscreen;
    
    if (!CreateWindow(config)) {
        glfwTerminate();
        return false;
    }
    
    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_FATAL("Failed to initialize GLAD");
        glfwDestroyWindow(m_window);
        glfwTerminate();
        return false;
    }
    
    // Set callbacks
    glfwSetFramebufferSizeCallback(m_window, FramebufferSizeCallback);
    glfwSetWindowFocusCallback(m_window, WindowFocusCallback);
    
    // Store this pointer for callbacks
    glfwSetWindowUserPointer(m_window, this);
    
    // Set VSync
    SetVSync(config.vsync);
    
    // Log OpenGL info
    LOG_INFO("OpenGL Vendor: " + std::string((char*)glGetString(GL_VENDOR)));
    LOG_INFO("OpenGL Renderer: " + std::string((char*)glGetString(GL_RENDERER)));
    LOG_INFO("OpenGL Version: " + std::string((char*)glGetString(GL_VERSION)));
    LOG_INFO("GLSL Version: " + std::string((char*)glGetString(GL_SHADING_LANGUAGE_VERSION)));
    
    LOG_INFO("Window initialized successfully");
    return true;
}

bool Window::CreateWindow(const WindowConfig& config) {
    GLFWmonitor* monitor = nullptr;
    
    if (config.fullscreen) {
        monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        m_width = mode->width;
        m_height = mode->height;
    }
    
    m_window = glfwCreateWindow(m_width, m_height, config.title.c_str(), monitor, nullptr);
    
    if (!m_window) {
        LOG_FATAL("Failed to create GLFW window");
        return false;
    }
    
    glfwMakeContextCurrent(m_window);
    
    if (!config.fullscreen) {
        CenterWindow();
    }
    
    return true;
}

void Window::CenterWindow() {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    
    int windowX = (mode->width - m_width) / 2;
    int windowY = (mode->height - m_height) / 2;
    
    glfwSetWindowPos(m_window, windowX, windowY);
}

void Window::Shutdown() {
    if (m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
    glfwTerminate();
    LOG_INFO("Window shutdown");
}

void Window::SwapBuffers() {
    glfwSwapBuffers(m_window);
}

void Window::PollEvents() {
    glfwPollEvents();
}

bool Window::ShouldClose() const {
    return glfwWindowShouldClose(m_window);
}

void Window::SetShouldClose(bool close) {
    glfwSetWindowShouldClose(m_window, close);
}

void Window::SetVSync(bool enabled) {
    glfwSwapInterval(enabled ? 1 : 0);
}

void Window::SetFullscreen(bool fullscreen) {
    if (m_fullscreen == fullscreen) return;
    
    m_fullscreen = fullscreen;
    
    if (fullscreen) {
        m_windowedWidth = m_width;
        m_windowedHeight = m_height;
        
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        
        glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        m_width = mode->width;
        m_height = mode->height;
    } else {
        glfwSetWindowMonitor(m_window, nullptr, 
            (m_windowedWidth - m_width) / 2, 
            (m_windowedHeight - m_height) / 2,
            m_windowedWidth, m_windowedHeight, 0);
        m_width = m_windowedWidth;
        m_height = m_windowedHeight;
        CenterWindow();
    }
}

void Window::SetTitle(const std::string& title) {
    m_title = title;
    glfwSetWindowTitle(m_window, title.c_str());
}

void Window::SetSize(int width, int height) {
    m_width = width;
    m_height = height;
    glfwSetWindowSize(m_window, width, height);
}

void Window::SetResizeCallback(ResizeCallback callback) {
    m_resizeCallback = callback;
}

void Window::SetFocusCallback(FocusCallback callback) {
    m_focusCallback = callback;
}

void Window::ShowCursor(bool show) {
    glfwSetInputMode(m_window, GLFW_CURSOR, show ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

void Window::SetCursorPosition(double x, double y) {
    glfwSetCursorPos(m_window, x, y);
}

void Window::GetCursorPosition(double& x, double& y) const {
    glfwGetCursorPos(m_window, &x, &y);
}

void Window::FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win) {
        win->m_width = width;
        win->m_height = height;
        glViewport(0, 0, width, height);
        
        if (win->m_resizeCallback) {
            win->m_resizeCallback(width, height);
        }
    }
}

void Window::WindowFocusCallback(GLFWwindow* window, int focused) {
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win) {
        win->m_focused = (focused == GLFW_TRUE);
        
        if (win->m_focusCallback) {
            win->m_focusCallback(win->m_focused);
        }
    }
}

} // namespace fps::core
