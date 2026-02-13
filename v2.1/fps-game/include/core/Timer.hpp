#pragma once

#include <chrono>
#include <vector>
#include <string>

namespace fps::core {

class Timer {
public:
    Timer();
    
    void Reset();
    double GetElapsedSeconds() const;
    double GetElapsedMilliseconds() const;
    double GetElapsedMicroseconds() const;
    
private:
    std::chrono::high_resolution_clock::time_point m_start;
};

class DeltaTime {
public:
    DeltaTime();
    
    void Update();
    float GetDeltaTime() const { return m_deltaTime; }
    float GetFPS() const { return m_fps; }
    double GetTotalTime() const { return m_totalTime; }
    
    void SetTargetFPS(int fps);
    void CapFrameRate();
    
private:
    std::chrono::high_resolution_clock::time_point m_lastFrame;
    std::chrono::high_resolution_clock::time_point m_currentFrame;
    float m_deltaTime;
    float m_fps;
    double m_totalTime;
    int m_targetFPS;
    float m_targetFrameTime;
    
    std::vector<float> m_frameTimeHistory;
    static constexpr size_t MAX_HISTORY = 60;
};

class Stopwatch {
public:
    Stopwatch(const std::string& name = "Stopwatch");
    
    void Start();
    void Stop();
    void Reset();
    double GetElapsedMilliseconds() const;
    
    void PrintElapsed() const;
    
private:
    std::string m_name;
    std::chrono::high_resolution_clock::time_point m_startTime;
    double m_elapsedMs;
    bool m_running;
};

} // namespace fps::core
