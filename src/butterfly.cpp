#include "butterfly.h"
#include <iostream>
#include <random>
#include <cmath>
#include <GLFW/glfw3.h>

// Random number generation
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

Butterfly::Butterfly(Shader& shader) 
    : shader(shader), 
      position(0.0f, 1.5f, 0.0f),
      direction(1.0f, 0.0f, 0.0f),
      wingAngle(0.0f),
      wingSpeed(10.0f),
      flightSpeed(1.0f),
      timeSinceDirectionChange(0.0f) {
    
    // Generate butterfly geometry
    GenerateGeometry();
    
    // Set up OpenGL buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // Initial random direction
    UpdateDirection();
}

Butterfly::~Butterfly() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Butterfly::Update(float deltaTime) {
    // Update wing flapping
    wingAngle += wingSpeed * deltaTime;
    
    // Update position
    position += direction * flightSpeed * deltaTime;
    
    // Change direction occasionally
    timeSinceDirectionChange += deltaTime;
    if (timeSinceDirectionChange > 3.0f) {
        float chance = dist(gen);
        if (chance > 0.7f) {
            UpdateDirection();
        }
        timeSinceDirectionChange = 0.0f;
    }
    
    // Simple boundary checking
    if (position.x > 10.0f || position.x < -10.0f || 
        position.z > 10.0f || position.z < -10.0f) {
        direction = glm::normalize(glm::vec3(-position.x, 0.0f, -position.z));
    }
    
    // Keep butterfly at a reasonable height
    position.y = 1.5f + sin(glfwGetTime() * 0.5f) * 0.3f;
}

void Butterfly::Draw(const glm::mat4& view, const glm::mat4& projection) {
    shader.use();
    
    // Calculate model matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    
    // Make butterfly face the direction it's moving
    float angle = atan2(direction.x, direction.z);
    model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
    
    // Apply wing flapping animation
    float leftWingAngle = sin(wingAngle) * 0.5f + 0.5f;  // 0 to 1
    float rightWingAngle = -sin(wingAngle) * 0.5f + 0.5f; // 1 to 0
    
    // Set shader uniforms
    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    shader.setFloat("leftWingAngle", leftWingAngle);
    shader.setFloat("rightWingAngle", rightWingAngle);
    
    // Bind VAO and set up vertex attributes
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Draw butterfly
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 6); // 6 floats per vertex (3 position, 3 color)
    
    // Unbind VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Butterfly::GenerateGeometry() {
    // Butterfly body (a thin box)
    float bodyLength = 0.5f;
    float bodyWidth = 0.1f;
    float bodyHeight = 0.1f;
    
    // Colors
    glm::vec3 bodyColor(0.2f, 0.2f, 0.2f);  // Dark gray
    glm::vec3 wingColor(0.8f, 0.2f, 0.8f);   // Purple
    
    // Body vertices (a thin box)
    // Front face
    AddQuad(
        glm::vec3(-bodyLength/2, -bodyHeight/2, -bodyWidth/2),
        glm::vec3(bodyLength/2, -bodyHeight/2, -bodyWidth/2),
        glm::vec3(bodyLength/2, bodyHeight/2, -bodyWidth/2),
        glm::vec3(-bodyLength/2, bodyHeight/2, -bodyWidth/2),
        bodyColor
    );
    
    // Back face
    AddQuad(
        glm::vec3(-bodyLength/2, -bodyHeight/2, bodyWidth/2),
        glm::vec3(bodyLength/2, -bodyHeight/2, bodyWidth/2),
        glm::vec3(bodyLength/2, bodyHeight/2, bodyWidth/2),
        glm::vec3(-bodyLength/2, bodyHeight/2, bodyWidth/2),
        bodyColor
    );
    
    // Left wing (will be animated)
    float wingSize = 0.5f;
    glm::vec3 wingOrigin(-bodyLength/4, 0.0f, 0.0f);
    
    // Left wing (top)
    AddTriangle(
        wingOrigin,
        wingOrigin + glm::vec3(-wingSize, 0.0f, 0.0f),
        wingOrigin + glm::vec3(0.0f, wingSize, 0.0f),
        wingColor
    );
    
    // Left wing (bottom)
    AddTriangle(
        wingOrigin,
        wingOrigin + glm::vec3(-wingSize, 0.0f, 0.0f),
        wingOrigin + glm::vec3(0.0f, -wingSize, 0.0f),
        wingColor
    );
    
    // Right wing (top)
    AddTriangle(
        -wingOrigin,
        -wingOrigin + glm::vec3(wingSize, 0.0f, 0.0f),
        -wingOrigin + glm::vec3(0.0f, wingSize, 0.0f),
        wingColor
    );
    
    // Right wing (bottom)
    AddTriangle(
        -wingOrigin,
        -wingOrigin + glm::vec3(wingSize, 0.0f, 0.0f),
        -wingOrigin + glm::vec3(0.0f, -wingSize, 0.0f),
        wingColor
    );
}

void Butterfly::AddVertex(const glm::vec3& position, const glm::vec3& color) {
    // Add position
    vertices.push_back(position.x);
    vertices.push_back(position.y);
    vertices.push_back(position.z);
    // Add color
    vertices.push_back(color.r);
    vertices.push_back(color.g);
    vertices.push_back(color.b);
}

void Butterfly::AddTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& color) {
    AddVertex(p1, color);
    AddVertex(p2, color);
    AddVertex(p3, color);
}

void Butterfly::AddQuad(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& p4, const glm::vec3& color) {
    // First triangle
    AddTriangle(p1, p2, p3, color);
    // Second triangle
    AddTriangle(p1, p3, p4, color);
}

void Butterfly::UpdateDirection() {
    direction = GetRandomDirection();
}

glm::vec3 Butterfly::GetRandomDirection() {
    // Generate a random direction in the XZ plane
    float angle = dist(gen) * 3.14159f * 2.0f;
    return glm::normalize(glm::vec3(cos(angle), 0.0f, sin(angle)));
}
