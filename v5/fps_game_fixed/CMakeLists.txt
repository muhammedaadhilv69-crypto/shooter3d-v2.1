cmake_minimum_required(VERSION 3.10)
project(FPSGame VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler flags
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
endif()

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ---- SDL2 from source ----
set(SDL_SHARED ON  CACHE BOOL "Build SDL2 as shared library" FORCE)
set(SDL_STATIC OFF CACHE BOOL "Don't build static SDL2"      FORCE)
set(SDL_TEST   OFF CACHE BOOL "Don't build SDL2 tests"       FORCE)
add_subdirectory(external/SDL2)

# ---- OpenGL ----
find_package(OpenGL REQUIRED)

# ---- Include paths ----
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/external/SDL2/include
    ${CMAKE_SOURCE_DIR}/external/glm
    ${CMAKE_SOURCE_DIR}/external/glad/include
    ${OPENGL_INCLUDE_DIR}
)

# ---- Source files ----
set(ENGINE_SOURCES
    src/engine/Shader.cpp
    src/engine/Camera.cpp
    src/engine/Renderer.cpp
    src/engine/Texture.cpp
    src/engine/Mesh.cpp
    src/engine/Window.cpp
)

set(GAME_SOURCES
    src/game/Player.cpp
    src/game/Weapon.cpp
    src/game/Enemy.cpp
    src/game/Level.cpp
    src/game/Game.cpp
    src/game/Particle.cpp
)

# ---- glad loader (C file) ----
set(GLAD_SOURCES
    external/glad/src/glad.c
)

# ---- Executable ----
add_executable(FPSGame WIN32
    src/main.cpp
    ${ENGINE_SOURCES}
    ${GAME_SOURCES}
    ${GLAD_SOURCES}
)

# ---- Link libraries ----
target_link_libraries(FPSGame
    SDL2main
    SDL2
    ${OPENGL_LIBRARIES}
)

# Windows-specific
if(WIN32)
    target_link_libraries(FPSGame opengl32)
    # Copy SDL2.dll next to the exe
    add_custom_command(TARGET FPSGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL2>
            $<TARGET_FILE_DIR:FPSGame>
    )
endif()

# Linux-specific
if(UNIX AND NOT APPLE)
    target_link_libraries(FPSGame GL dl)
endif()

# macOS-specific
if(APPLE)
    target_link_libraries(FPSGame "-framework OpenGL")
endif()

# Copy resources
add_custom_command(TARGET FPSGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:FPSGame>/resources
)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Building FPS Game with glad OpenGL loader + SDL2 from source")
