#include "core/Timer.hpp"
#include <algorithm>

namespace fps::core {

// Timer Implementation
Timer::Timer() {
    Reset();
}

void Timer::Reset() {
    m_start = std::chrono::high_resolution_clock::now();
}

double Timer::GetElapsedSeconds() const {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(now - m_start);
    return duration.count() / 1e9;
}

double Timer::GetElapsedMilliseconds() const {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - m_start);
    return duration.count() / 1000.0;
}

double Timer::GetElapsedMicroseconds() const {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - m_start);
    return static_cast<double>(duration.count());
}

// DeltaTime Implementation
DeltaTime::DeltaTime()
    : m_deltaTime(0.0f)
    , m_fps(0.0f)
    , m_totalTime(0.0)
    , m_targetFPS(60)
    , m_targetFrameTime(1.0f / 60.0f) {
    m_lastFrame = std::chrono::high_resolution_clock::now();
    m_frameTimeHistory.reserve(MAX_HISTORY);
}

void DeltaTime::Update() {
    m_currentFrame = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(m_currentFrame - m_lastFrame);
    m_deltaTime = static_cast<float>(duration.count() / 1e9);
    
    // Prevent spiral of death
    if (m_deltaTime > 0.1f) {
        m_deltaTime = 0.1f;
    }
    
    m_totalTime += m_deltaTime;
    
    // Calculate FPS
    m_frameTimeHistory.push_back(m_deltaTime);
    if (m_frameTimeHistory.size() > MAX_HISTORY) {
        m_frameTimeHistory.erase(m_frameTimeHistory.begin());
    }
    
    float avgFrameTime = 0.0f;
    for (float ft : m_frameTimeHistory) {
        avgFrameTime += ft;
    }
    avgFrameTime /= static_cast<float>(m_frameTimeHistory.size());
    m_fps = 1.0f / avgFrameTime;
    
    m_lastFrame = m_currentFrame;
}

void DeltaTime::SetTargetFPS(int fps) {
    m_targetFPS = fps;
    m_targetFrameTime = 1.0f / static_cast<float>(fps);
}

void DeltaTime::CapFrameRate() {
    float frameTime = m_deltaTime;
    if (frameTime < m_targetFrameTime) {
        float sleepTime = m_targetFrameTime - frameTime;
        auto sleepDuration = std::chrono::nanoseconds(static_cast<long long>(sleepTime * 1e9));
        std::this_thread::sleep_for(sleepDuration);
    }
}

// Stopwatch Implementation
Stopwatch::Stopwatch(const std::string& name)
    : m_name(name)
    , m_elapsedMs(0.0)
    , m_running(false) {
}

void Stopwatch::Start() {
    m_startTime = std::chrono::high_resolution_clock::now();
    m_running = true;
}

void Stopwatch::Stop() {
    if (m_running) {
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - m_startTime);
        m_elapsedMs += duration.count() / 1000.0;
        m_running = false;
    }
}

void Stopwatch::Reset() {
    m_elapsedMs = 0.0;
    m_running = false;
}

double Stopwatch::GetElapsedMilliseconds() const {
    if (m_running) {
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - m_startTime);
        return m_elapsedMs + duration.count() / 1000.0;
    }
    return m_elapsedMs;
}

void Stopwatch::PrintElapsed() const {
    double ms = GetElapsedMilliseconds();
    if (ms < 1.0) {
        std::cout << m_name << ": " << ms * 1000.0 << " us" << std::endl;
    } else if (ms < 1000.0) {
        std::cout << m_name << ": " << ms << " ms" << std::endl;
    } else {
        std::cout << m_name << ": " << ms / 1000.0 << " s" << std::endl;
    }
}

} // namespace fps::core
