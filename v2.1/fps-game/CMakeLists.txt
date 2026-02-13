cmake_minimum_required(VERSION 3.16)
project(FPSShooter VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Options
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(ENABLE_AUDIO "Enable OpenAL audio" ON)
option(ENABLE_DEBUG "Enable debug mode" OFF)

# Platform detection
if(WIN32)
    set(PLATFORM_WINDOWS TRUE)
    add_definitions(-DPLATFORM_WINDOWS)
elseif(UNIX AND NOT APPLE)
    set(PLATFORM_LINUX TRUE)
    add_definitions(-DPLATFORM_LINUX)
endif()

# Compiler flags for production grade
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX- /O2 /fp:fast /EHsc")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi /Od /DEBUG")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -O3 -ffast-math -march=native")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -DDEBUG")
endif()

if(ENABLE_DEBUG)
    add_definitions(-DDEBUG)
endif()

# Find packages
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(third_party/glfw)

# GLAD
add_library(glad STATIC third_party/glad/src/glad.c)
target_include_directories(glad PUBLIC third_party/glad/include)

# GLM (header only)
add_subdirectory(third_party/glm)

# stb_image
add_library(stb_image STATIC third_party/stb/stb_image_impl.c)
target_include_directories(stb_image PUBLIC third_party/stb)

# OpenAL (optional)
if(ENABLE_AUDIO)
    find_package(OpenAL QUIET)
    if(OPENAL_FOUND)
        add_definitions(-DHAS_AUDIO)
    endif()
endif()

# Source files
set(CORE_SOURCES
    src/core/Window.cpp
    src/core/Input.cpp
    src/core/Timer.cpp
    src/core/Logger.cpp
)

set(MATH_SOURCES
    src/math/Math.cpp
)

set(RENDERER_SOURCES
    src/renderer/Shader.cpp
    src/renderer/Texture.cpp
    src/renderer/Mesh.cpp
    src/renderer/Model.cpp
    src/renderer/Camera.cpp
    src/renderer/Renderer.cpp
    src/renderer/Framebuffer.cpp
    src/renderer/ParticleSystem.cpp
)

set(GAME_SOURCES
    src/game/Player.cpp
    src/game/Weapon.cpp
    src/game/Projectile.cpp
    src/game/Enemy.cpp
    src/game/GameWorld.cpp
    src/game/Level.cpp
    src/game/HUD.cpp
)

set(AUDIO_SOURCES
    src/audio/AudioManager.cpp
)

set(UTILS_SOURCES
    src/utils/ResourceManager.cpp
    src/utils/Config.cpp
)

set(MAIN_SOURCE
    src/main.cpp
)

# Executable
add_executable(${PROJECT_NAME} 
    ${MAIN_SOURCE}
    ${CORE_SOURCES}
    ${MATH_SOURCES}
    ${RENDERER_SOURCES}
    ${GAME_SOURCES}
    ${AUDIO_SOURCES}
    ${UTILS_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    include
    third_party/glad/include
    third_party/stb
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    glad
    glm::glm
    stb_image
    OpenGL::GL
    Threads::Threads
)

if(OPENAL_FOUND AND ENABLE_AUDIO)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenAL::OpenAL)
endif()

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE winmm)
endif()

# Copy assets
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)

# Installation
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(DIRECTORY assets DESTINATION bin)

# Print configuration
message(STATUS "")
message(STATUS "=== FPSShooter Configuration ===")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Audio: ${ENABLE_AUDIO}")
message(STATUS "Debug: ${ENABLE_DEBUG}")
message(STATUS "================================")
message(STATUS "")
