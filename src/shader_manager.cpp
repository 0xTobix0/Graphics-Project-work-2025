#include "shader_manager.h"
#include "shader.h"
#include <memory>
#include <unordered_map>
#include <string>
#include <iostream>

// Global shader pointers
ShaderPtr ourShader;
ShaderPtr skyboxShader;
ShaderPtr lightShader;
ShaderPtr butterflyShader;
ShaderPtr textShader;

// Map of shader names to shader pointers
static std::unordered_map<std::string, ShaderPtr> shaderCache;

void initShaders() {
    // Clean up any existing shaders first
    cleanupShaders();
    
    // Initialize shaders with full paths and correct extensions
    std::string shaderPath = "shaders/";
    
    try {
        // Create shaders using ShaderPtr
        ourShader = std::make_shared<Shader>(
            (shaderPath + "vertex_shader.vert").c_str(), 
            (shaderPath + "fragment_shader.frag").c_str()
        );
        
        skyboxShader = std::make_shared<Shader>(
            (shaderPath + "skybox.vert").c_str(), 
            (shaderPath + "skybox.frag").c_str()
        );
        
        lightShader = std::make_shared<Shader>(
            (shaderPath + "vertex_shader.vert").c_str(), 
            (shaderPath + "fragment_shader.frag").c_str()
        );
        
        butterflyShader = std::make_shared<Shader>(
            (shaderPath + "butterfly.vert").c_str(),
            (shaderPath + "butterfly.frag").c_str()
        );
        
        // Initialize text shader
        textShader = std::make_shared<Shader>(
            (shaderPath + "text.vert").c_str(),
            (shaderPath + "text.frag").c_str()
        );
        
        // Add shaders to cache
        shaderCache["default"] = ourShader;
        shaderCache["skybox"] = skyboxShader;
        shaderCache["light"] = lightShader;
        shaderCache["butterfly"] = butterflyShader;
        shaderCache["text"] = textShader;  // Make sure text shader is in cache
        
        // Cache all shaders
        shaderCache["default"] = ourShader;
        shaderCache["skybox"] = skyboxShader;
        shaderCache["light"] = lightShader;
        shaderCache["butterfly"] = butterflyShader;
        shaderCache["text"] = textShader;
        
    } catch (const std::exception& e) {
        std::cerr << "Error initializing shaders: " << e.what() << std::endl;
        cleanupShaders();
        throw;
    }
}

void cleanupShaders()
{
    // Reset all shader pointers (automatically deletes if last reference)
    ourShader.reset();
    skyboxShader.reset();
    lightShader.reset();
    butterflyShader.reset();
    textShader.reset();
    shaderCache.clear();
    
    // Clear any OpenGL errors
    while(glGetError() != GL_NO_ERROR);
}

ShaderPtr GetShader(const std::string& name) {
    auto it = shaderCache.find(name);
    if (it != shaderCache.end()) {
        return it->second;
    }
    return nullptr;
}

// Initialize shader manager
void InitializeShaderManager() {
    initShaders();
}
