#ifndef BOX_H
#define BOX_H

#include "../external/glad-3.3/include/glad/gl.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>

class Shader;

class Box {
public:
    struct InstanceData {
        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec3 color;
        float scale;
        float rotation;
        float rotationSpeed;
        bool isLightSource;  // Whether this box is a light source
        
        InstanceData(const glm::vec3& pos, const glm::vec3& col, float scl, bool isLight = false)
            : position(pos), color(col), scale(scl), rotation(0.0f), isLightSource(isLight) {
            // Random velocity
            velocity = glm::vec3(
                (rand() % 100 - 50) * 0.001f,  // -0.05 to 0.05
                (rand() % 100 - 50) * 0.001f,
                (rand() % 100 - 50) * 0.001f
            );
            // Random rotation speed
            rotationSpeed = (rand() % 100) * 0.001f + 0.001f; // 0.001 to 0.1
        }
        
        void update(float deltaTime) {
            // Skip update for light sources to keep them fixed
            if (isLightSource) {
                return;
            }
            
            // Update position for non-light boxes
            position += velocity * deltaTime * 60.0f; // Scale by delta time and a speed factor
            
            // Update rotation
            rotation += rotationSpeed * deltaTime * 60.0f;
            if (rotation > 360.0f) rotation -= 360.0f;
            
            // Simple boundary checking (optional: make boxes wrap around)
            const float boundary = 10.0f;
            if (position.x < -boundary) position.x = boundary;
            if (position.x > boundary) position.x = -boundary;
            if (position.y < -boundary) position.y = boundary;
            if (position.y > boundary) position.y = -boundary;
            if (position.z < -boundary) position.z = boundary;
            if (position.z > boundary) position.z = -boundary;
        }
    };

    static void addInstance(const InstanceData& instance);
    static void clearInstances();
    static void setupBuffers();
    static void cleanup();
    static void updateInstances(float deltaTime);
    static void drawInstances(Shader& shader, const glm::mat4& view, const glm::mat4& projection, float time);
    
private:
    static void drawCube(Shader& shader, const glm::mat4& model, const glm::vec3& color, float rotation = 0.0f);
    static std::vector<InstanceData> instances;
    static bool buffersInitialized;
    static GLuint VAO, VBO, EBO;
    
    // Initialize the cube's VAO, VBO, and EBO
    static void initCube();
    // Draw a single cube
    static void drawCube(Shader& shader, const glm::mat4& model, const glm::vec3& color);
};

#endif // BOX_H
