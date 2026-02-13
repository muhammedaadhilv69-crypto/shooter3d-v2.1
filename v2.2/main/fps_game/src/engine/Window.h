#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif
#include <SDL_opengl.h>
#include <string>
#include <functional>

namespace Engine {

class Window {
public:
    Window(const std::string& title, int width, int height);
    ~Window();

    bool init();
    void processEvents();
    void swap();
    void shutdown();

    bool isRunning() const { return m_running; }
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    
    void setMouseCallback(std::function<void(double, double)> callback) {
        m_mouseCallback = callback;
    }
    
    void setKeyCallback(std::function<void(int, bool)> callback) {
        m_keyCallback = callback;
    }
    
    void captureMouse(bool capture);
    float getDeltaTime() const { return m_deltaTime; }
    
private:
    SDL_Window* m_window;
    SDL_GLContext m_context;
    std::string m_title;
    int m_width;
    int m_height;
    bool m_running;
    float m_deltaTime;
    Uint32 m_lastTime;
    bool m_mouseCaptured;
    
    std::function<void(double, double)> m_mouseCallback;
    std::function<void(int, bool)> m_keyCallback;
};

} // namespace Engine

#endif // WINDOW_H
