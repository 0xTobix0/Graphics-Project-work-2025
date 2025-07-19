#ifndef BUTTERFLY_H
#define BUTTERFLY_H

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <cmath>
#include "shader.h"

class Butterfly {
public:
    // Constructor/Destructor
    Butterfly(Shader& shader);
    ~Butterfly();
    
    // Update butterfly state (position, wing flapping, etc.)
    void Update(float deltaTime);
    
    // Draw the butterfly
    void Draw(const glm::mat4& view, const glm::mat4& projection);
    
private:
    // Butterfly properties
    glm::vec3 position;
    glm::vec3 direction;
    float wingAngle;
    float wingSpeed;
    float flightSpeed;
    float timeSinceDirectionChange;
    
    // OpenGL objects
    unsigned int VAO, VBO;
    Shader& shader;
    
    // Butterfly geometry data
    std::vector<float> vertices;
    
    // Helper methods
    void GenerateGeometry();
    void UpdateDirection();
    glm::vec3 GetRandomDirection();
    void AddVertex(const glm::vec3& position, const glm::vec3& color);
    void AddTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& color);
    void AddQuad(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& p4, const glm::vec3& color);
};

#endif // BUTTERFLY_H
