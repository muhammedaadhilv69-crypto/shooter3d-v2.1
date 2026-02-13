#pragma once

#include <string>
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace fps::core {

struct WindowConfig {
    int width = 1280;
    int height = 720;
    std::string title = "FPS Shooter";
    bool fullscreen = false;
    bool vsync = true;
    int msaaSamples = 4;
    bool resizable = false;
    int openglMajor = 3;
    int openglMinor = 3;
};

class Window {
public:
    using ResizeCallback = std::function<void(int, int)>;
    using FocusCallback = std::function<void(bool)>;
    
    Window();
    ~Window();
    
    bool Initialize(const WindowConfig& config);
    void Shutdown();
    
    void SwapBuffers();
    void PollEvents();
    bool ShouldClose() const;
    void SetShouldClose(bool close);
    
    void SetVSync(bool enabled);
    void SetFullscreen(bool fullscreen);
    void SetTitle(const std::string& title);
    void SetSize(int width, int height);
    
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    float GetAspectRatio() const { return static_cast<float>(m_width) / static_cast<float>(m_height); }
    
    GLFWwindow* GetNativeWindow() { return m_window; }
    
    void SetResizeCallback(ResizeCallback callback);
    void SetFocusCallback(FocusCallback callback);
    
    void ShowCursor(bool show);
    void SetCursorPosition(double x, double y);
    void GetCursorPosition(double& x, double& y) const;
    
    bool IsFullscreen() const { return m_fullscreen; }
    bool IsFocused() const { return m_focused; }
    
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void WindowFocusCallback(GLFWwindow* window, int focused);
    
private:
    GLFWwindow* m_window;
    int m_width;
    int m_height;
    int m_windowedWidth;
    int m_windowedHeight;
    bool m_fullscreen;
    bool m_focused;
    std::string m_title;
    
    ResizeCallback m_resizeCallback;
    FocusCallback m_focusCallback;
    
    bool CreateWindow(const WindowConfig& config);
    void CenterWindow();
};

} // namespace fps::core
