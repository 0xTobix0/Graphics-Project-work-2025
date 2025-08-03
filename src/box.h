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
        glm::vec3 color;
        float scale;
    };

    static void addInstance(const InstanceData& instance);
    static void clearInstances();
    static void setupBuffers();
    static void cleanup();
    static void drawInstances(Shader& shader, const glm::mat4& view, const glm::mat4& projection, float time);

private:
    static std::vector<InstanceData> instances;
    static bool buffersInitialized;
    static GLuint VAO, VBO, EBO;
    
    // Initialize the cube's VAO, VBO, and EBO
    static void initCube();
    // Draw a single cube
    static void drawCube(Shader& shader, const glm::mat4& model, const glm::vec3& color);
};

#endif // BOX_H
