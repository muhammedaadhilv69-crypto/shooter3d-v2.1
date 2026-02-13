#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "renderer/Mesh.hpp"
#include "renderer/Texture.hpp"
#include "math/Math.hpp"

namespace fps::game {

struct LevelObject {
    std::string name;
    math::Transform transform;
    std::shared_ptr<renderer::Mesh> mesh;
    std::shared_ptr<renderer::Texture2D> texture;
    std::shared_ptr<renderer::Texture2D> normalMap;
    
    bool collidable;
    math::AABB bounds;
    
    LevelObject() : collidable(true) {}
};

struct SpawnPoint {
    glm::vec3 position;
    float yaw;
    std::string type; // "player", "enemy", "weapon", etc.
};

class Level {
public:
    Level();
    ~Level();
    
    bool LoadFromFile(const std::string& filepath);
    bool GenerateProcedural(int seed, int complexity);
    void Clear();
    
    void Render(const glm::mat4& viewProj);
    void RenderCollisionGeometry();
    
    // Queries
    bool Raycast(const math::Ray& ray, float maxDistance, math::RaycastHit& outHit);
    bool CheckCollision(const math::AABB& bounds);
    bool CheckCollision(const math::Sphere& sphere);
    
    std::vector<LevelObject*> GetObjectsInRadius(const glm::vec3& position, float radius);
    
    // Spawn points
    glm::vec3 GetPlayerSpawn() const;
    std::vector<glm::vec3> GetEnemySpawnPoints() const;
    std::vector<glm::vec3> GetItemSpawnPoints() const;
    
    // Bounds
    math::AABB GetBounds() const { return m_bounds; }
    bool IsPointInBounds(const glm::vec3& point) const;
    
    // Floor height at position
    float GetFloorHeight(const glm::vec3& position) const;
    
    // Navigation
    glm::vec3 FindRandomReachablePoint(const glm::vec3& start, float maxDistance) const;
    std::vector<glm::vec3> FindPath(const glm::vec3& start, const glm::vec3& end) const;
    
private:
    std::vector<std::unique_ptr<LevelObject>> m_objects;
    std::vector<SpawnPoint> m_spawnPoints;
    math::AABB m_bounds;
    
    // Collision grid for optimization
    static constexpr float CELL_SIZE = 5.0f;
    std::unordered_map<std::pair<int, int>, std::vector<LevelObject*>, PairHash> m_collisionGrid;
    
    void BuildCollisionGrid();
    std::pair<int, int> GetGridCell(const glm::vec3& position) const;
    
    void GenerateRoom(const glm::vec3& position, const glm::vec3& size);
    void GenerateCorridor(const glm::vec3& start, const glm::vec3& end, float width);
    void GenerateWalls();
    void GenerateFloor();
    void GenerateCeiling();
    
    void CalculateBounds();
    void OptimizeGeometry();
};

} // namespace fps::game
