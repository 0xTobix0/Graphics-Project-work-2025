#include "skybox.h"
#include "shader.h"
#include <vector>
#include <iostream>

// Include OpenGL headers
#include "glad/gl.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Skybox vertices
float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

Skybox::Skybox(const std::vector<std::string>& faces, Shader& shader) 
    : shader(shader) {
    // Generate VAO and VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    // Bind the VAO first, then bind and set VBOs and attribute pointers
    glBindVertexArray(VAO);
    
    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // Upload vertex data to the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    
    // Set the vertex attribute pointers (position attribute)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Unbind the VBO first, then unbind the VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // Check for OpenGL errors
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error in Skybox constructor: " << err << std::endl;
    }
    
    // Load cubemap texture
    cubemapTexture = loadCubemap(faces);
    
    if (cubemapTexture == 0) {
        std::cerr << "Failed to load cubemap textures" << std::endl;
    }
}

Skybox::~Skybox() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &cubemapTexture);
}

void Skybox::Draw(glm::mat4 view, glm::mat4 projection) {
    // Save current depth function state
    GLint oldDepthFunc;
    glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFunc);
    
    // Change depth function for skybox rendering
    glDepthFunc(GL_LEQUAL);
    
    // Use the shader program
    shader.use();
    
    // Bind the VAO (which has the VBO and attribute pointers already set up)
    glBindVertexArray(VAO);
    
    // Remove translation from the view matrix for skybox
    view = glm::mat4(glm::mat3(view));
    
    // Set shader uniforms
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    
    // Bind cubemap texture to texture unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    shader.setInt("skybox", 0);  // Make sure the shader knows to use texture unit 0
    
    // Draw the skybox
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // Check for OpenGL errors after drawing
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error in Skybox::Draw: " << err << std::endl;
    }
    
    // Cleanup - unbind VAO first, then texture
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    
    // Restore original depth function
    glDepthFunc(oldDepthFunc);
}

// Define STB_IMAGE_IMPLEMENTATION in exactly one source file
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Forward declaration of stb_image functions
extern "C" {
    unsigned char* stbi_load(char const *filename, int *x, int *y, int *comp, int req_comp);
    void stbi_image_free(void *retval_from_stbi_load);
}

unsigned int Skybox::loadCubemap(const std::vector<std::string>& faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    // Load each face of the cubemap
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        std::cout << "Loading cubemap texture: " << faces[i] << std::endl;
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            std::cout << "  Success! Dimensions: " << width << "x" << height 
                      << ", Channels: " << nrChannels << std::endl;
            GLenum format = GL_RGB;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;
                
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            std::cerr << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            // Fall back to a solid color if loading fails
            unsigned char color[3] = {100, 149, 237}; // Cornflower blue
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, 1, 1, 0, 
                         GL_RGB, GL_UNSIGNED_BYTE, color);
        }
    }
    
    return textureID;
}
