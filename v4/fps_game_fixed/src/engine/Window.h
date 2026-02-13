#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <SDL2/SDL.h>
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
    void captureMouse(bool capture);

    bool isRunning() const { return m_running; }
    float getDeltaTime() const { return m_deltaTime; }
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }

    void setKeyCallback(std::function<void(int, bool)> cb) { m_keyCallback = cb; }
    void setMouseCallback(std::function<void(double, double)> cb) { m_mouseCallback = cb; }

private:
    SDL_Window*   m_window;
    SDL_GLContext m_context;
    std::string   m_title;
    int           m_width;
    int           m_height;
    bool          m_running;
    float         m_deltaTime;
    Uint32        m_lastTime;
    bool          m_mouseCaptured;

    std::function<void(int, bool)>       m_keyCallback;
    std::function<void(double, double)>  m_mouseCallback;
};

} // namespace Engine

#endif // WINDOW_H
