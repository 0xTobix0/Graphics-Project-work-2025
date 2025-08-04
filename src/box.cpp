#include "box.h"
#include "shader.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// Debug logging macro
#define LOG_DEBUG(message) std::cout << "[Box Debug] " << message << std::endl

// Initialize static members
std::vector<Box::InstanceData> Box::instances;
bool Box::buffersInitialized = false;
GLuint Box::VAO = 0;
GLuint Box::VBO = 0;
GLuint Box::EBO = 0;

// Cube vertices with positions and normals (interleaved)
const float cubeVertices[] = {
    // Positions          // Normals
    // Front face
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  // 0
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  // 1
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  // 2
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  // 3
    // Back face
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  // 4
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  // 5
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  // 6
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  // 7
    // Left face
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  // 8 (same as 4)
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  // 9 (same as 0)
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  // 10 (same as 3)
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  // 11 (same as 7)
    // Right face
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  // 12 (same as 1)
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  // 13 (same as 5)
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  // 14 (same as 6)
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  // 15 (same as 2)
    // Bottom face
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  // 16 (same as 4)
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  // 17 (same as 5)
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  // 18 (same as 1)
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  // 19 (same as 0)
    // Top face
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  // 20 (same as 3)
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  // 21 (same as 2)
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  // 22 (same as 6)
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f   // 23 (same as 7)
};

// Indices for the cube (6 faces, 2 triangles per face, 3 vertices per triangle)
const unsigned int cubeIndices[] = {
    // Front face
    0, 1, 2,
    2, 3, 0,
    // Right face
    1, 5, 6,
    6, 2, 1,
    // Back face
    5, 4, 7,
    7, 6, 5,
    // Left face
    4, 0, 3,
    3, 7, 4,
    // Bottom face
    4, 5, 1,
    1, 0, 4,
    // Top face
    3, 2, 6,
    6, 7, 3
};

// Add a box instance
void Box::addInstance(const InstanceData& instance) {
    instances.push_back(instance);
}

void Box::clearInstances() {
    instances.clear();
}

void Box::setupBuffers() {
    if (!buffersInitialized) {
        initCube();
        buffersInitialized = true;
    }
}

void Box::cleanup() {
    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        VAO = VBO = EBO = 0;
        buffersInitialized = false;
    }
}

void Box::initCube() {
    if (VAO != 0) {
        LOG_DEBUG("Cube already initialized, skipping");
        return; // Already initialized
    }
    
    LOG_DEBUG("Initializing cube VAO, VBO, and EBO");
    
    // Generate and bind the VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    LOG_DEBUG("Created VAO: " << VAO);
    
    // Generate and bind the VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    LOG_DEBUG("Created VBO with " << sizeof(cubeVertices) << " bytes of vertex data");
    
    // Generate and bind the EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);
    LOG_DEBUG("Created EBO with " << sizeof(cubeIndices) << " bytes of index data (" << (sizeof(cubeIndices)/sizeof(unsigned int)) << " indices)");
    
    // Set the vertex attribute pointers
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Check for OpenGL errors
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        LOG_DEBUG("OpenGL error in initCube: " << err);
    }
    
    // Unbind the VAO (good practice)
    glBindVertexArray(0);
    
    LOG_DEBUG("Cube initialization complete");
}

void Box::drawCube(Shader& shader, const glm::mat4& model, const glm::vec3& color, float rotation) {
    // Create a rotation matrix
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.5f, 1.0f, 0.3f));
    
    // Combine rotation with the model matrix
    glm::mat4 finalModel = model * rotationMatrix;
    
    // Set the model matrix and color
    shader.setMat4("model", finalModel);
    shader.setVec3("instanceColor", color);
    
    // Draw the cube
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

// Update all instances
void Box::updateInstances(float deltaTime) {
    for (auto& instance : instances) {
        instance.update(deltaTime);
    }
}

// Draw all instances using modern OpenGL
void Box::drawInstances(Shader& shader, const glm::mat4& view, const glm::mat4& projection, float time) {
    if (instances.empty()) {
        return;
    }
    
    // Make sure buffers are set up
    setupBuffers();
    
    // Use the shader
    shader.use();
    
    // Set up view and projection matrices
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    
    // Set view position (camera position) for lighting calculations
    glm::vec3 viewPos = glm::vec3(glm::inverse(view)[3]);
    shader.setVec3("viewPos", viewPos);
    
    // Find the light source (if any)
    glm::vec3 lightPos(0.0f, 10.0f, 0.0f); // Default light position (above the scene)
    bool hasLightSource = false;
    
    // First pass: find the light source
    for (const auto& instance : instances) {
        if (instance.isLightSource) {
            lightPos = instance.position;
            hasLightSource = true;
            break;
        }
    }
    
    // Bind the VAO
    glBindVertexArray(VAO);
    
    // Draw each instance
    for (const auto& instance : instances) {
        // Skip light source in the first pass
        if (instance.isLightSource) {
            continue;
        }
        
        // Create model matrix for this instance
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, instance.position);
        model = glm::scale(model, glm::vec3(instance.scale));
        model = glm::rotate(model, instance.rotation, glm::vec3(0.0f, 1.0f, 0.0f));
        
        // Set the model matrix and color
        shader.setMat4("model", model);
        shader.setVec3("instanceColor", instance.color);
        shader.setBool("isLightSource", false);
        shader.setVec3("lightPos", lightPos);
        
        // Draw the cube
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
    
    // Second pass: draw light sources
    if (hasLightSource) {
        // Use a different shader or set a flag for light sources
        for (const auto& instance : instances) {
            if (!instance.isLightSource) {
                continue;
            }
            
            // Create model matrix for the light source
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, instance.position);
            model = glm::scale(model, glm::vec3(instance.scale));
            
            // Set the model matrix and color
            shader.setMat4("model", model);
            shader.setVec3("instanceColor", instance.color);
            shader.setBool("isLightSource", true);
            
            // Draw the light source
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }
    }
    
    // Unbind VAO
    glBindVertexArray(0);
}
