#include "core/Input.hpp"
#include "core/Logger.hpp"
#include <GLFW/glfw3.h>

namespace fps::core {

Input& Input::GetInstance() {
    static Input instance;
    return instance;
}

void Input::Initialize(GLFWwindow* window) {
    m_window = window;
    m_firstMouse = true;
    
    // Check for raw mouse motion support
    m_rawInputSupported = glfwRawMouseMotionSupported();
    if (m_rawInputSupported) {
        LOG_INFO("Raw mouse motion is supported");
    }
    
    // Set GLFW callbacks
    glfwSetKeyCallback(window, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
        Input& input = Input::GetInstance();
        if (action == GLFW_PRESS) {
            input.SetKeyState(key, true);
        } else if (action == GLFW_RELEASE) {
            input.SetKeyState(key, false);
        }
    });
    
    glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int button, int action, int mods) {
        Input& input = Input::GetInstance();
        if (action == GLFW_PRESS) {
            input.SetMouseButtonState(button, true);
        } else if (action == GLFW_RELEASE) {
            input.SetMouseButtonState(button, false);
        }
    });
    
    glfwSetScrollCallback(window, [](GLFWwindow* w, double xoffset, double yoffset) {
        Input& input = Input::GetInstance();
        input.SetMouseScrollDelta(static_cast<float>(yoffset));
    });
    
    // Initialize arrays
    m_currentKeyState.fill(false);
    m_previousKeyState.fill(false);
    m_currentMouseState.fill(false);
    m_previousMouseState.fill(false);
    
    LOG_INFO("Input system initialized");
}

void Input::Update() {
    // Store previous states
    m_previousKeyState = m_currentKeyState;
    m_previousMouseState = m_currentMouseState;
    
    // Reset scroll delta
    m_mouseScrollDelta = 0.0f;
    
    // Update mouse position
    double x, y;
    glfwGetCursorPos(m_window, &x, &y);
    
    if (m_firstMouse) {
        m_previousMousePos.x = static_cast<float>(x);
        m_previousMousePos.y = static_cast<float>(y);
        m_firstMouse = false;
    }
    
    m_currentMousePos.x = static_cast<float>(x);
    m_currentMousePos.y = static_cast<float>(y);
    
    m_mouseDelta = m_currentMousePos - m_previousMousePos;
    m_previousMousePos = m_currentMousePos;
}

void Input::Shutdown() {
    LOG_INFO("Input system shutdown");
}

void Input::SetKeyState(int key, bool pressed) {
    if (key >= 0 && key < MAX_KEYS) {
        m_currentKeyState[key] = pressed;
    }
}

void Input::SetMouseButtonState(int button, bool pressed) {
    if (button >= 0 && button < MAX_MOUSE_BUTTONS) {
        m_currentMouseState[button] = pressed;
    }
}

void Input::SetMouseScrollDelta(float delta) {
    m_mouseScrollDelta += delta;
}

bool Input::IsKeyPressed(KeyCode key) const {
    int k = static_cast<int>(key);
    return k >= 0 && k < MAX_KEYS && m_currentKeyState[k] && !m_previousKeyState[k];
}

bool Input::IsKeyDown(KeyCode key) const {
    int k = static_cast<int>(key);
    return k >= 0 && k < MAX_KEYS && m_currentKeyState[k];
}

bool Input::IsKeyReleased(KeyCode key) const {
    int k = static_cast<int>(key);
    return k >= 0 && k < MAX_KEYS && !m_currentKeyState[k] && m_previousKeyState[k];
}

bool Input::IsMouseButtonPressed(MouseButton button) const {
    int b = static_cast<int>(button);
    return b >= 0 && b < MAX_MOUSE_BUTTONS && m_currentMouseState[b] && !m_previousMouseState[b];
}

bool Input::IsMouseButtonDown(MouseButton button) const {
    int b = static_cast<int>(button);
    return b >= 0 && b < MAX_MOUSE_BUTTONS && m_currentMouseState[b];
}

bool Input::IsMouseButtonReleased(MouseButton button) const {
    int b = static_cast<int>(button);
    return b >= 0 && b < MAX_MOUSE_BUTTONS && !m_currentMouseState[b] && m_previousMouseState[b];
}

glm::vec2 Input::GetMousePosition() const {
    return m_currentMousePos;
}

glm::vec2 Input::GetMouseDelta() const {
    return m_mouseDelta;
}

float Input::GetMouseScrollDelta() const {
    return m_mouseScrollDelta;
}

void Input::SetMousePosition(float x, float y) {
    glfwSetCursorPos(m_window, x, y);
    m_currentMousePos.x = x;
    m_currentMousePos.y = y;
    m_previousMousePos = m_currentMousePos;
}

void Input::SetMouseVisible(bool visible) {
    glfwSetInputMode(m_window, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    if (!visible) {
        m_firstMouse = true;
    }
}

void Input::SetMouseRawInput(bool enabled) {
    if (m_rawInputSupported) {
        glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, enabled ? GLFW_TRUE : GLFW_FALSE);
    }
}

} // namespace fps::core
