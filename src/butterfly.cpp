#include "butterfly.h"
#include "obj_loader.h"
#include <iostream>
#include <random>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

// Random number generation
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

Butterfly::Butterfly(Shader& shader, const std::string& modelPath) 
    : shader(shader), animationTime(0.0f) {
    // Initialize butterfly properties
    position = glm::vec3(0.0f, 1.5f, -5.0f);  // Position further back in the scene
    direction = GetRandomDirection();
    wingAngle = 0.0f;
    wingSpeed = 5.0f;
    flightSpeed = 0.5f;
    scale = 0.01f;  // Reduced scale to make the butterfly smaller
    timeSinceDirectionChange = 0.0f;
    
    // Initialize the OBJ loader with the butterfly model
    model = std::make_unique<OBJLoader>(shader);
    if (model) {
        std::cout << "Loading butterfly model from: " << modelPath << std::endl;
        if (!model->LoadModel(modelPath)) {
            std::cerr << "Failed to load butterfly model: " << modelPath << std::endl;
        } else {
            std::cout << "Successfully loaded butterfly model" << std::endl;
        }
    } else {
        std::cerr << "Failed to create OBJLoader instance" << std::endl;
    }
}

Butterfly::~Butterfly() = default;

void Butterfly::Update(float deltaTime) {
    // Update position based on direction and speed
    position += direction * flightSpeed * deltaTime;
    
    // Update wing flapping animation
    wingAngle += wingSpeed * deltaTime;
    animationTime += deltaTime;
    
    // Randomly change direction occasionally
    timeSinceDirectionChange += deltaTime;
    if (timeSinceDirectionChange > 3.0f) {
        if (std::rand() % 100 < 5) {  // 5% chance to change direction each second after 3 seconds
            UpdateDirection();
            timeSinceDirectionChange = 0.0f;
        }
    }
    
    // Keep butterfly within bounds
    const float boundary = 10.0f;
    if (abs(position.x) > boundary || abs(position.z) > boundary) {
        direction = glm::normalize(glm::vec3(-position.x, 0.0f, -position.z));
    }
    
    // Keep butterfly at reasonable height
    if (position.y < 0.5f) {
        position.y = 0.5f;
        direction.y = abs(direction.y);
    } else if (position.y > 5.0f) {
        position.y = 5.0f;
        direction.y = -abs(direction.y);
    }
}

void Butterfly::Draw(const glm::mat4& view, const glm::mat4& projection) {
    if (!model) {
        std::cerr << "Butterfly::Draw: No model to draw!" << std::endl;
        return;
    }
    
    static int frameCount = 0;
    frameCount++;
    
    // Only print debug info every 60 frames to avoid flooding the console
    if (frameCount % 60 == 0) {
        std::cout << "\n=== Butterfly Debug Info ===" << std::endl;
        std::cout << "Position: (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
        
        glm::mat4 modelMatrix = GetModelMatrix();
        // Extract translation (last column of the matrix)
        glm::vec3 translation = glm::vec3(modelMatrix[3]);
        
        // Extract scale (length of the basis vectors)
        glm::vec3 scale;
        scale.x = glm::length(glm::vec3(modelMatrix[0]));
        scale.y = glm::length(glm::vec3(modelMatrix[1]));
        scale.z = glm::length(glm::vec3(modelMatrix[2]));
        
        std::cout << "Model Scale: (" << scale.x << ", " << scale.y << ", " << scale.z << ")" << std::endl;
        std::cout << "Model Translation: (" << translation.x << ", " << translation.y << ", " << translation.z << ")" << std::endl;
        
        // Get view position (camera position)
        glm::vec3 viewPos = glm::vec3(glm::inverse(view)[3]);
        std::cout << "Camera Position: (" << viewPos.x << ", " << viewPos.y << ", " << viewPos.z << ")" << std::endl;
        
        // Calculate distance from camera
        float distance = glm::distance(translation, viewPos);
        std::cout << "Distance from camera: " << distance << std::endl;
    }
    
    // Use the shader
    shader.use();
    
    // Set view position (extract from view matrix)
    glm::vec3 viewPos = glm::vec3(glm::inverse(view)[3]);
    shader.setVec3("viewPos", viewPos);
    
    // Set up model matrix
    glm::mat4 modelMatrix = GetModelMatrix();
    shader.setMat4("model", modelMatrix);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    
    // Set light position to be above and slightly in front of the camera
    glm::vec3 lightPos = viewPos + glm::vec3(2.0f, 3.0f, 2.0f);
    shader.setVec3("light.position", lightPos);
    
    // Enhanced light properties for better visibility
    shader.setVec3("light.ambient", 0.3f, 0.3f, 0.3f);  // Increased ambient
    shader.setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);  // Brighter diffuse
    shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f); // Keep specular bright
    
    // Set material properties (in case they're not set in the model)
    shader.setVec3("material.ambient", 1.0f, 1.0f, 1.0f);
    shader.setVec3("material.diffuse", 0.8f, 0.8f, 0.8f);
    shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    shader.setFloat("material.shininess", 32.0f);
    
    // Debug output (uncomment if needed)
    // std::cout << "Light position: (" << lightPos.x << ", " << lightPos.y << ", " << lightPos.z << ")" << std::endl;
    // std::cout << "View position: (" << viewPos.x << ", " << viewPos.y << ", " << viewPos.z << ")" << std::endl;
    
    // Calculate wing angles for animation (reduced amplitude for debugging)
    float leftWingAngle = 0.2f * sin(wingAngle);
    float rightWingAngle = 0.2f * sin(wingAngle + 3.14159f); // Opposite phase for right wing
    
    // Set wing animation uniforms
    shader.setFloat("leftWingAngle", leftWingAngle);
    shader.setFloat("rightWingAngle", rightWingAngle);
    
    // Calculate normal matrix
    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
    shader.setMat3("normalMatrix", normalMatrix);
    
    // Draw the model
    model->Draw(shader);
}

glm::mat4 Butterfly::GetModelMatrix() const {
    // Start with identity matrix
    glm::mat4 model = glm::mat4(1.0f);
    
    // Apply translation (position in world space)
    model = glm::translate(model, position);
    
    // Rotate the model to face the camera (simplified for now)
    // This assumes the model's forward is along the -Z axis
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    // Apply uniform scaling
    model = glm::scale(model, glm::vec3(scale));
    
    // Debug output (uncomment if needed)
    // std::cout << "Butterfly position: (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
    // std::cout << "Butterfly scale: " << scale << std::endl;
    
    return model;
}

void Butterfly::UpdateDirection() {
    // Slightly randomize the current direction
    direction = glm::normalize(direction + GetRandomDirection() * 0.3f);
}

glm::vec3 Butterfly::GetRandomDirection() {
    // Generate a random direction in the XZ plane
    float angle = dist(gen) * 3.14159f * 2.0f;
    return glm::normalize(glm::vec3(cos(angle), 0.0f, sin(angle)));
}
