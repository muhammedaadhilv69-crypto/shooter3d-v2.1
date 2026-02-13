#include <iostream>
#include <memory>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core/Window.hpp"
#include "core/Input.hpp"
#include "core/Timer.hpp"
#include "core/Logger.hpp"

#include "renderer/Shader.hpp"
#include "renderer/Mesh.hpp"
#include "renderer/Texture.hpp"
#include "renderer/Camera.hpp"
#include "renderer/Renderer.hpp"

#include "math/Math.hpp"

using namespace fps;

// Global game state
bool g_running = true;
bool g_paused = false;
float g_deltaTime = 0.0f;
float g_totalTime = 0.0f;

// Game objects
struct Player {
    renderer::Camera camera;
    glm::vec3 velocity{0.0f};
    float speed = 5.0f;
    float sprintSpeed = 10.0f;
    float mouseSensitivity = 0.1f;
    float pitch = 0.0f;
    float yaw = -90.0f;
    bool grounded = true;
    float health = 100.0f;
    
    void Update(float dt, core::Window& window) {
        HandleInput(dt, window);
        UpdateCamera();
    }
    
    void HandleInput(float dt, core::Window& window) {
        auto& input = core::Input::GetInstance();
        
        // Mouse look
        glm::vec2 mouseDelta = input.GetMouseDelta();
        yaw += mouseDelta.x * mouseSensitivity;
        pitch -= mouseDelta.y * mouseSensitivity;
        pitch = math::Clamp(pitch, -89.0f, 89.0f);
        
        // Movement
        glm::vec3 direction(0.0f);
        glm::vec3 forward = camera.GetForward();
        glm::vec3 right = camera.GetRight();
        
        // Flatten forward vector for movement
        forward.y = 0.0f;
        forward = math::Normalize(forward);
        
        float currentSpeed = input.IsKeyDown(core::KeyCode::LeftShift) ? sprintSpeed : speed;
        
        if (input.IsKeyDown(core::KeyCode::W)) direction += forward;
        if (input.IsKeyDown(core::KeyCode::S)) direction -= forward;
        if (input.IsKeyDown(core::KeyCode::A)) direction -= right;
        if (input.IsKeyDown(core::KeyCode::D)) direction += right;
        
        if (glm::length(direction) > 0.0f) {
            direction = math::Normalize(direction);
            camera.Move(direction * currentSpeed * dt);
        }
        
        // Vertical movement (simple fly mode for now)
        if (input.IsKeyDown(core::KeyCode::Space)) {
            camera.Move(glm::vec3(0.0f, 1.0f, 0.0f) * currentSpeed * dt);
        }
        if (input.IsKeyDown(core::KeyCode::LeftControl)) {
            camera.Move(glm::vec3(0.0f, -1.0f, 0.0f) * currentSpeed * dt);
        }
    }
    
    void UpdateCamera() {
        camera.SetEulerAngles(pitch, yaw);
    }
};

struct Enemy {
    math::Transform transform;
    float health = 50.0f;
    float speed = 2.0f;
    bool alive = true;
    glm::vec3 color{1.0f, 0.0f, 0.0f};
    
    void Update(float dt, const glm::vec3& targetPos) {
        if (!alive) return;
        
        glm::vec3 dir = targetPos - transform.position;
        float dist = glm::length(dir);
        
        if (dist > 1.0f) {
            dir = math::Normalize(dir);
            transform.position += dir * speed * dt;
            transform.LookAt(targetPos);
        }
    }
};

struct Projectile {
    glm::vec3 position;
    glm::vec3 direction;
    float speed = 50.0f;
    float damage = 25.0f;
    float lifetime = 2.0f;
    float age = 0.0f;
    bool active = true;
    
    void Update(float dt) {
        if (!active) return;
        
        position += direction * speed * dt;
        age += dt;
        
        if (age >= lifetime) {
            active = false;
        }
    }
};

// Game systems
class SimpleFPSGame {
public:
    std::unique_ptr<core::Window> window;
    std::unique_ptr<Player> player;
    std::unique_ptr<renderer::Shader> worldShader;
    std::unique_ptr<renderer::Shader> enemyShader;
    std::unique_ptr<renderer::Mesh> cubeMesh;
    std::unique_ptr<renderer::Mesh> floorMesh;
    std::vector<Enemy> enemies;
    std::vector<Projectile> projectiles;
    
    core::DeltaTime deltaTime;
    
    bool Initialize() {
        // Initialize window
        window = std::make_unique<core::Window>();
        core::WindowConfig config;
        config.width = 1280;
        config.height = 720;
        config.title = "FPS Shooter - OpenGL 3.3";
        config.vsync = false;
        
        if (!window->Initialize(config)) {
            LOG_FATAL("Failed to initialize window");
            return false;
        }
        
        // Initialize input
        core::Input::GetInstance().Initialize(window->GetNativeWindow());
        core::Input::GetInstance().SetMouseVisible(false);
        core::Input::GetInstance().SetMouseRawInput(true);
        
        // Initialize renderer
        if (!renderer::Renderer::GetInstance().Initialize(window->GetWidth(), window->GetHeight())) {
            LOG_FATAL("Failed to initialize renderer");
            return false;
        }
        
        // Create player
        player = std::make_unique<Player>();
        player->camera.SetPosition(glm::vec3(0.0f, 2.0f, 5.0f));
        player->camera.SetPerspective(75.0f, window->GetAspectRatio(), 0.1f, 1000.0f);
        
        // Load shaders
        worldShader = std::make_unique<renderer::Shader>();
        const std::string worldVert = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec3 aNormal;
            layout (location = 2) in vec2 aTexCoord;
            
            out vec3 FragPos;
            out vec3 Normal;
            out vec2 TexCoord;
            
            uniform mat4 model;
            uniform mat4 view;
            uniform mat4 projection;
            
            void main() {
                FragPos = vec3(model * vec4(aPos, 1.0));
                Normal = mat3(transpose(inverse(model))) * aNormal;
                TexCoord = aTexCoord;
                gl_Position = projection * view * vec4(FragPos, 1.0);
            }
        )";
        
        const std::string worldFrag = R"(
            #version 330 core
            out vec4 FragColor;
            
            in vec3 FragPos;
            in vec3 Normal;
            in vec2 TexCoord;
            
            uniform vec3 objectColor;
            uniform vec3 lightPos;
            uniform vec3 lightColor;
            uniform vec3 viewPos;
            
            void main() {
                // Ambient
                float ambientStrength = 0.3;
                vec3 ambient = ambientStrength * lightColor;
                
                // Diffuse
                vec3 norm = normalize(Normal);
                vec3 lightDir = normalize(lightPos - FragPos);
                float diff = max(dot(norm, lightDir), 0.0);
                vec3 diffuse = diff * lightColor;
                
                // Specular
                float specularStrength = 0.5;
                vec3 viewDir = normalize(viewPos - FragPos);
                vec3 reflectDir = reflect(-lightDir, norm);
                float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
                vec3 specular = specularStrength * spec * lightColor;
                
                vec3 result = (ambient + diffuse + specular) * objectColor;
                FragColor = vec4(result, 1.0);
            }
        )";
        
        worldShader->LoadFromSource(worldVert, worldFrag);
        
        // Enemy shader (simpler, for performance)
        enemyShader = std::make_unique<renderer::Shader>();
        const std::string enemyVert = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec3 aNormal;
            
            out vec3 FragPos;
            out vec3 Normal;
            
            uniform mat4 model;
            uniform mat4 view;
            uniform mat4 projection;
            
            void main() {
                FragPos = vec3(model * vec4(aPos, 1.0));
                Normal = mat3(transpose(inverse(model))) * aNormal;
                gl_Position = projection * view * vec4(FragPos, 1.0);
            }
        )";
        
        const std::string enemyFrag = R"(
            #version 330 core
            out vec4 FragColor;
            
            in vec3 FragPos;
            in vec3 Normal;
            
            uniform vec3 enemyColor;
            uniform vec3 lightPos;
            
            void main() {
                vec3 norm = normalize(Normal);
                vec3 lightDir = normalize(lightPos - FragPos);
                float diff = max(dot(norm, lightDir), 0.0);
                
                vec3 ambient = 0.3 * enemyColor;
                vec3 diffuse = diff * enemyColor;
                
                FragColor = vec4(ambient + diffuse, 1.0);
            }
        )";
        
        enemyShader->LoadFromSource(enemyVert, enemyFrag);
        
        // Create meshes
        cubeMesh = std::make_unique<renderer::Mesh>();
        cubeMesh->CreateCube();
        
        floorMesh = std::make_unique<renderer::Mesh>();
        std::vector<renderer::Vertex> floorVerts = {
            {{-50.0f, 0.0f, -50.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
            {{ 50.0f, 0.0f, -50.0f}, {0.0f, 1.0f, 0.0f}, {50.0f, 0.0f}},
            {{ 50.0f, 0.0f,  50.0f}, {0.0f, 1.0f, 0.0f}, {50.0f, 50.0f}},
            {{-50.0f, 0.0f,  50.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 50.0f}}
        };
        std::vector<unsigned int> floorIndices = {0, 1, 2, 0, 2, 3};
        floorMesh->Create(floorVerts, floorIndices);
        
        // Spawn initial enemies
        SpawnEnemies(5);
        
        LOG_INFO("Game initialized successfully");
        return true;
    }
    
    void SpawnEnemies(int count) {
        for (int i = 0; i < count; i++) {
            Enemy enemy;
            float angle = (float)i / count * 2.0f * M_PI;
            float radius = 15.0f + (rand() % 10);
            enemy.transform.position = glm::vec3(
                cos(angle) * radius,
                1.0f,
                sin(angle) * radius
            );
            enemies.push_back(enemy);
        }
    }
    
    void FireWeapon() {
        Projectile proj;
        proj.position = player->camera.GetPosition();
        proj.direction = player->camera.GetForward();
        projectiles.push_back(proj);
        
        LOG_DEBUG("Fired projectile");
    }
    
    void Update() {
        deltaTime.Update();
        g_deltaTime = deltaTime.GetDeltaTime();
        g_totalTime += g_deltaTime;
        
        // Update input
        core::Input::GetInstance().Update();
        
        // Check for exit
        if (core::Input::GetInstance().IsKeyPressed(core::KeyCode::Escape)) {
            g_running = false;
            return;
        }
        
        // Fire weapon
        if (core::Input::GetInstance().IsMouseButtonPressed(core::MouseButton::Left)) {
            FireWeapon();
        }
        
        // Update player
        player->Update(g_deltaTime, *window);
        
        // Update enemies
        for (auto& enemy : enemies) {
            enemy.Update(g_deltaTime, player->camera.GetPosition());
        }
        
        // Update projectiles
        for (auto& proj : projectiles) {
            proj.Update(g_deltaTime);
        }
        
        // Remove inactive projectiles
        projectiles.erase(
            std::remove_if(projectiles.begin(), projectiles.end(),
                [](const Projectile& p) { return !p.active; }),
            projectiles.end()
        );
        
        // Collision detection (projectiles vs enemies)
        for (auto& proj : projectiles) {
            if (!proj.active) continue;
            
            for (auto& enemy : enemies) {
                if (!enemy.alive) continue;
                
                float dist = glm::distance(proj.position, enemy.transform.position);
                if (dist < 1.5f) {
                    enemy.health -= proj.damage;
                    proj.active = false;
                    
                    if (enemy.health <= 0.0f) {
                        enemy.alive = false;
                        LOG_INFO("Enemy killed!");
                    }
                    break;
                }
            }
        }
        
        // Remove dead enemies and respawn
        size_t aliveCount = 0;
        for (const auto& enemy : enemies) {
            if (enemy.alive) aliveCount++;
        }
        
        if (aliveCount == 0) {
            enemies.clear();
            SpawnEnemies(5 + (rand() % 5));
            LOG_INFO("New wave spawned!");
        }
        
        // Poll window events
        window->PollEvents();
        if (window->ShouldClose()) {
            g_running = false;
        }
    }
    
    void Render() {
        auto& renderer = renderer::Renderer::GetInstance();
        
        // Clear screen
        renderer.Clear(glm::vec4(0.1f, 0.1f, 0.15f, 1.0f));
        
        renderer.BeginFrame(player->camera);
        
        glm::mat4 view = player->camera.GetViewMatrix();
        glm::mat4 projection = player->camera.GetProjectionMatrix();
        glm::vec3 viewPos = player->camera.GetPosition();
        glm::vec3 lightPos(10.0f, 20.0f, 10.0f);
        
        // Render floor
        worldShader->Bind();
        worldShader->SetMat4("view", view);
        worldShader->SetMat4("projection", projection);
        worldShader->SetVec3("lightPos", lightPos);
        worldShader->SetVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        worldShader->SetVec3("viewPos", viewPos);
        
        glm::mat4 floorModel = glm::mat4(1.0f);
        worldShader->SetMat4("model", floorModel);
        worldShader->SetVec3("objectColor", glm::vec3(0.3f, 0.3f, 0.35f));
        floorMesh->Draw(*worldShader);
        
        // Render some obstacles
        for (int i = 0; i < 5; i++) {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(i * 5.0f - 10.0f, 1.0f, -10.0f));
            model = glm::scale(model, glm::vec3(2.0f, 4.0f, 2.0f));
            worldShader->SetMat4("model", model);
            worldShader->SetVec3("objectColor", glm::vec3(0.5f, 0.5f, 0.5f));
            cubeMesh->Draw(*worldShader);
        }
        
        // Render enemies
        enemyShader->Bind();
        enemyShader->SetMat4("view", view);
        enemyShader->SetMat4("projection", projection);
        enemyShader->SetVec3("lightPos", lightPos);
        
        for (const auto& enemy : enemies) {
            if (!enemy.alive) continue;
            
            glm::mat4 model = enemy.transform.GetMatrix();
            model = glm::scale(model, glm::vec3(1.0f, 2.0f, 1.0f));
            enemyShader->SetMat4("model", model);
            enemyShader->SetVec3("enemyColor", enemy.color);
            cubeMesh->Draw(*enemyShader);
        }
        
        // Render projectiles
        worldShader->Bind();
        for (const auto& proj : projectiles) {
            if (!proj.active) continue;
            
            glm::mat4 model = glm::translate(glm::mat4(1.0f), proj.position);
            model = glm::scale(model, glm::vec3(0.1f));
            worldShader->SetMat4("model", model);
            worldShader->SetVec3("objectColor", glm::vec3(1.0f, 0.8f, 0.0f));
            cubeMesh->Draw(*worldShader);
        }
        
        renderer.EndFrame();
        
        // Swap buffers
        window->SwapBuffers();
    }
    
    void Shutdown() {
        renderer::Renderer::GetInstance().Shutdown();
        window->Shutdown();
        LOG_INFO("Game shutdown");
    }
    
    void Run() {
        while (g_running) {
            Update();
            Render();
        }
    }
};

int main(int argc, char** argv) {
    LOG_INFO("=== FPS Shooter Starting ===");
    LOG_INFO("OpenGL 3.3 / DirectX 10 Equivalent");
    
    SimpleFPSGame game;
    
    if (!game.Initialize()) {
        LOG_FATAL("Failed to initialize game");
        return -1;
    }
    
    game.Run();
    game.Shutdown();
    
    LOG_INFO("=== FPS Shooter Exited ===");
    return 0;
}
