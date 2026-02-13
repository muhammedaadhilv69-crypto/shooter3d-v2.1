#pragma once

#include <array>
#include <unordered_map>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

namespace fps::core {

// Key codes (GLFW compatible)
enum class KeyCode {
    Unknown = GLFW_KEY_UNKNOWN,
    Space = GLFW_KEY_SPACE,
    Apostrophe = GLFW_KEY_APOSTROPHE,
    Comma = GLFW_KEY_COMMA,
    Minus = GLFW_KEY_MINUS,
    Period = GLFW_KEY_PERIOD,
    Slash = GLFW_KEY_SLASH,
    Num0 = GLFW_KEY_0,
    Num1 = GLFW_KEY_1,
    Num2 = GLFW_KEY_2,
    Num3 = GLFW_KEY_3,
    Num4 = GLFW_KEY_4,
    Num5 = GLFW_KEY_5,
    Num6 = GLFW_KEY_6,
    Num7 = GLFW_KEY_7,
    Num8 = GLFW_KEY_8,
    Num9 = GLFW_KEY_9,
    Semicolon = GLFW_KEY_SEMICOLON,
    Equal = GLFW_KEY_EQUAL,
    A = GLFW_KEY_A,
    B = GLFW_KEY_B,
    C = GLFW_KEY_C,
    D = GLFW_KEY_D,
    E = GLFW_KEY_E,
    F = GLFW_KEY_F,
    G = GLFW_KEY_G,
    H = GLFW_KEY_H,
    I = GLFW_KEY_I,
    J = GLFW_KEY_J,
    K = GLFW_KEY_K,
    L = GLFW_KEY_L,
    M = GLFW_KEY_M,
    N = GLFW_KEY_N,
    O = GLFW_KEY_O,
    P = GLFW_KEY_P,
    Q = GLFW_KEY_Q,
    R = GLFW_KEY_R,
    S = GLFW_KEY_S,
    T = GLFW_KEY_T,
    U = GLFW_KEY_U,
    V = GLFW_KEY_V,
    W = GLFW_KEY_W,
    X = GLFW_KEY_X,
    Y = GLFW_KEY_Y,
    Z = GLFW_KEY_Z,
    LeftBracket = GLFW_KEY_LEFT_BRACKET,
    Backslash = GLFW_KEY_BACKSLASH,
    RightBracket = GLFW_KEY_RIGHT_BRACKET,
    GraveAccent = GLFW_KEY_GRAVE_ACCENT,
    Escape = GLFW_KEY_ESCAPE,
    Enter = GLFW_KEY_ENTER,
    Tab = GLFW_KEY_TAB,
    Backspace = GLFW_KEY_BACKSPACE,
    Insert = GLFW_KEY_INSERT,
    Delete = GLFW_KEY_DELETE,
    Right = GLFW_KEY_RIGHT,
    Left = GLFW_KEY_LEFT,
    Down = GLFW_KEY_DOWN,
    Up = GLFW_KEY_UP,
    PageUp = GLFW_KEY_PAGE_UP,
    PageDown = GLFW_KEY_PAGE_DOWN,
    Home = GLFW_KEY_HOME,
    End = GLFW_KEY_END,
    CapsLock = GLFW_KEY_CAPS_LOCK,
    ScrollLock = GLFW_KEY_SCROLL_LOCK,
    NumLock = GLFW_KEY_NUM_LOCK,
    PrintScreen = GLFW_KEY_PRINT_SCREEN,
    Pause = GLFW_KEY_PAUSE,
    F1 = GLFW_KEY_F1,
    F2 = GLFW_KEY_F2,
    F3 = GLFW_KEY_F3,
    F4 = GLFW_KEY_F4,
    F5 = GLFW_KEY_F5,
    F6 = GLFW_KEY_F6,
    F7 = GLFW_KEY_F7,
    F8 = GLFW_KEY_F8,
    F9 = GLFW_KEY_F9,
    F10 = GLFW_KEY_F10,
    F11 = GLFW_KEY_F11,
    F12 = GLFW_KEY_F12,
    Numpad0 = GLFW_KEY_KP_0,
    Numpad1 = GLFW_KEY_KP_1,
    Numpad2 = GLFW_KEY_KP_2,
    Numpad3 = GLFW_KEY_KP_3,
    Numpad4 = GLFW_KEY_KP_4,
    Numpad5 = GLFW_KEY_KP_5,
    Numpad6 = GLFW_KEY_KP_6,
    Numpad7 = GLFW_KEY_KP_7,
    Numpad8 = GLFW_KEY_KP_8,
    Numpad9 = GLFW_KEY_KP_9,
    NumpadDecimal = GLFW_KEY_KP_DECIMAL,
    NumpadDivide = GLFW_KEY_KP_DIVIDE,
    NumpadMultiply = GLFW_KEY_KP_MULTIPLY,
    NumpadSubtract = GLFW_KEY_KP_SUBTRACT,
    NumpadAdd = GLFW_KEY_KP_ADD,
    NumpadEnter = GLFW_KEY_KP_ENTER,
    NumpadEqual = GLFW_KEY_KP_EQUAL,
    LeftShift = GLFW_KEY_LEFT_SHIFT,
    LeftControl = GLFW_KEY_LEFT_CONTROL,
    LeftAlt = GLFW_KEY_LEFT_ALT,
    LeftSuper = GLFW_KEY_LEFT_SUPER,
    RightShift = GLFW_KEY_RIGHT_SHIFT,
    RightControl = GLFW_KEY_RIGHT_CONTROL,
    RightAlt = GLFW_KEY_RIGHT_ALT,
    RightSuper = GLFW_KEY_RIGHT_SUPER,
    Menu = GLFW_KEY_MENU
};

enum class MouseButton {
    Left = GLFW_MOUSE_BUTTON_LEFT,
    Right = GLFW_MOUSE_BUTTON_RIGHT,
    Middle = GLFW_MOUSE_BUTTON_MIDDLE,
    Button4 = GLFW_MOUSE_BUTTON_4,
    Button5 = GLFW_MOUSE_BUTTON_5,
    Button6 = GLFW_MOUSE_BUTTON_6,
    Button7 = GLFW_MOUSE_BUTTON_7,
    Button8 = GLFW_MOUSE_BUTTON_8
};

class Input {
public:
    static Input& GetInstance();
    
    void Initialize(GLFWwindow* window);
    void Update();
    void Shutdown();
    
    // Keyboard
    bool IsKeyPressed(KeyCode key) const;
    bool IsKeyDown(KeyCode key) const;
    bool IsKeyReleased(KeyCode key) const;
    
    // Mouse
    bool IsMouseButtonPressed(MouseButton button) const;
    bool IsMouseButtonDown(MouseButton button) const;
    bool IsMouseButtonReleased(MouseButton button) const;
    
    glm::vec2 GetMousePosition() const;
    glm::vec2 GetMouseDelta() const;
    float GetMouseScrollDelta() const;
    
    void SetMousePosition(float x, float y);
    void SetMouseVisible(bool visible);
    void SetMouseRawInput(bool enabled);
    
    // Callbacks (internal use)
    void SetKeyState(int key, bool pressed);
    void SetMouseButtonState(int button, bool pressed);
    void SetMouseScrollDelta(float delta);
    
private:
    Input() = default;
    ~Input() = default;
    
    Input(const Input&) = delete;
    Input& operator=(const Input&) = delete;
    
    static constexpr int MAX_KEYS = 512;
    static constexpr int MAX_MOUSE_BUTTONS = 8;
    
    GLFWwindow* m_window;
    
    std::array<bool, MAX_KEYS> m_currentKeyState{};
    std::array<bool, MAX_KEYS> m_previousKeyState{};
    
    std::array<bool, MAX_MOUSE_BUTTONS> m_currentMouseState{};
    std::array<bool, MAX_MOUSE_BUTTONS> m_previousMouseState{};
    
    glm::vec2 m_currentMousePos{0.0f};
    glm::vec2 m_previousMousePos{0.0f};
    glm::vec2 m_mouseDelta{0.0f};
    float m_mouseScrollDelta{0.0f};
    
    bool m_firstMouse{true};
    bool m_rawInputSupported{false};
};

} // namespace fps::core

// Convenience macros
#define INPUT fps::core::Input::GetInstance()
#define KEY_PRESSED(key) fps::core::Input::GetInstance().IsKeyPressed(fps::core::KeyCode::key)
#define KEY_DOWN(key) fps::core::Input::GetInstance().IsKeyDown(fps::core::KeyCode::key)
#define KEY_RELEASED(key) fps::core::Input::GetInstance().IsKeyReleased(fps::core::KeyCode::key)
#define MOUSE_PRESSED(btn) fps::core::Input::GetInstance().IsMouseButtonPressed(fps::core::MouseButton::btn)
#define MOUSE_DOWN(btn) fps::core::Input::GetInstance().IsMouseButtonDown(fps::core::MouseButton::btn)
#define MOUSE_RELEASED(btn) fps::core::Input::GetInstance().IsMouseButtonReleased(fps::core::MouseButton::btn)
