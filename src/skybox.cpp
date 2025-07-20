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
    : shader(shader), VAO(0), VBO(0), cubemapTexture(0) {
    // Initialize OpenGL objects
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    // Check for OpenGL errors after generating objects
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL error before VAO setup: " << err << std::endl;
    }
    
    // Bind the VAO first
    glBindVertexArray(VAO);
    
    // Bind and set up the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    
    // Set up vertex attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    // Unbind the VAO first, then the VBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // Check for OpenGL errors after setup
    err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL error after VAO setup: " << err << std::endl;
    }
    
    // Load cubemap texture
    cubemapTexture = loadCubemap(faces);
    
    if (cubemapTexture == 0) {
        std::cerr << "Failed to load cubemap textures" << std::endl;
    } else {
        std::cout << "Successfully loaded cubemap texture with ID: " << cubemapTexture << std::endl;
    }
}

Skybox::~Skybox() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(1, &cubemapTexture);
}

void Skybox::Draw(glm::mat4 view, glm::mat4 projection) {
    // Check if we have a valid VAO and texture
    if (VAO == 0 || cubemapTexture == 0) {
        std::cerr << "Skybox::Draw called with invalid VAO or texture" << std::endl;
        return;
    }
    
    // Save current depth function state
    GLint oldDepthFunc;
    glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFunc);
    
    // Change depth function for skybox rendering (draw at far plane)
    glDepthFunc(GL_LEQUAL);
    
    // Use the shader program
    shader.use();
    
    // Remove translation from the view matrix for skybox (only rotation matters)
    view = glm::mat4(glm::mat3(view));
    
    // Set shader uniforms
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    
    // Bind the VAO
    glBindVertexArray(VAO);
    
    // Bind cubemap texture to texture unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    shader.setInt("skybox", 0);  // Tell the shader to use texture unit 0
    
    // Draw the skybox
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // Check for OpenGL errors after drawing
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL error in Skybox::Draw after drawing: " << err << std::endl;
    }
    
    // Cleanup - unbind VAO first, then texture
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    
    // Restore original depth function
    glDepthFunc(oldDepthFunc);
    
    // Verify the depth function was restored correctly
    GLint currentDepthFunc;
    glGetIntegerv(GL_DEPTH_FUNC, &currentDepthFunc);
    if (currentDepthFunc != oldDepthFunc) {
        std::cerr << "Warning: Failed to restore original depth function" << std::endl;
    }
}

// STB image wrapper
#include "stb_image_wrapper.h"

// STB image implementation is now in stb_image_wrapper.h


unsigned int Skybox::loadCubemap(const std::vector<std::string>& faces) {
    unsigned int textureID = 0;
    
    // Generate and bind the cubemap texture
    glGenTextures(1, &textureID);
    if (textureID == 0) {
        std::cerr << "Failed to generate cubemap texture ID" << std::endl;
        return 0;
    }
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    // Check for OpenGL errors after setting parameters
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL error after setting cubemap parameters: " << err << std::endl;
        glDeleteTextures(1, &textureID);
        return 0;
    }
    
    // Load each face of the cubemap
    bool allFacesLoaded = true;
    int width = 0, height = 0, nrChannels = 0;
    
    for (unsigned int i = 0; i < faces.size(); i++) {
        std::cout << "Loading cubemap texture: " << faces[i] << std::endl;
        
        // Load image data
        stbi_set_flip_vertically_on_load(false); // Important for cubemaps
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        
        if (data) {
            std::cout << "  Success! Dimensions: " << width << "x" << height 
                      << ", Channels: " << nrChannels << std::endl;
            
            // Determine the format
            GLenum format = GL_RGB;
            if (nrChannels == 1) {
                format = GL_RED;
            } else if (nrChannels == 3) {
                format = GL_RGB;
            } else if (nrChannels == 4) {
                format = GL_RGBA;
            }
            
            // Upload the texture data
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                0,                  // mipmap level
                format,             // internal format
                width, height,      // dimensions
                0,                  // border (must be 0)
                format,             // format
                GL_UNSIGNED_BYTE,   // type
                data                // pixel data
            );
            
            // Check for errors after uploading texture data
            err = glGetError();
            if (err != GL_NO_ERROR) {
                std::cerr << "  OpenGL error uploading texture data for face " << i 
                          << ": " << err << std::endl;
                allFacesLoaded = false;
            }
            
            // Free the loaded image data
            stbi_image_free(data);
        } else {
            std::cerr << "  Cubemap texture failed to load at path: " << faces[i] << std::endl;
            allFacesLoaded = false;
            
            // Fall back to a solid color if loading fails
            unsigned char color[3] = {100, 149, 237}; // Cornflower blue
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                0, GL_RGB, 
                1, 1, 0, 
                GL_RGB, 
                GL_UNSIGNED_BYTE, 
                color
            );
        }
    }
    
    // Unbind the texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    
    // Check if all faces loaded successfully
    if (!allFacesLoaded) {
        std::cerr << "Warning: Not all cubemap faces loaded successfully" << std::endl;
    }
    
    return textureID;
}
