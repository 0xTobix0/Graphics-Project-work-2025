#ifndef BUTTERFLY_H
#define BUTTERFLY_H

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <cmath>
#include <memory>
#include <string>
#include "shader.h"

// Forward declaration to avoid including obj_loader.h here
class OBJLoader;

class Butterfly {
public:
    // Constructor/Destructor
    Butterfly(Shader& shader, const std::string& modelPath);
    ~Butterfly();
    
    // Update butterfly state (position, wing flapping, etc.)
    void Update(float deltaTime);
    
    // Draw the butterfly
    void Draw(const glm::mat4& view, const glm::mat4& projection);
    
    // Set/get position
    void SetPosition(const glm::vec3& pos) { position = pos; }
    glm::vec3 GetPosition() const { return position; }
    
    // Set/get scale
    void SetScale(float scale) { this->scale = scale; }
    float GetScale() const { return scale; }
    
private:
    // Butterfly properties
    glm::vec3 position;
    glm::vec3 direction;
    float wingAngle;
    float wingSpeed;
    float flightSpeed;
    float scale;
    float timeSinceDirectionChange;
    
    // Model and shader
    std::unique_ptr<OBJLoader> model;
    Shader& shader;
    
    // Animation state
    float animationTime;
    
    // Helper methods
    void UpdateDirection();
    glm::vec3 GetRandomDirection();
    glm::mat4 GetModelMatrix() const;
};

#endif // BUTTERFLY_H
