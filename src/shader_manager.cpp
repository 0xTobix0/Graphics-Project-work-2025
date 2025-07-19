#include "shader_manager.h"
#include "shader.h"
#include <memory>

// Initialize shader pointers with ShaderPtr for automatic memory management
ShaderPtr ourShader = nullptr;
ShaderPtr skyboxShader = nullptr;
ShaderPtr lightShader = nullptr;
ShaderPtr butterflyShader = nullptr;

void initShaders() {
    // Clean up any existing shaders first
    cleanupShaders();
    
    // Initialize shaders with full paths and correct extensions
    std::string shaderPath = "shaders/";
    
    // Create shaders using ShaderPtr
    ourShader = ShaderPtr(new Shader(
        (shaderPath + "vertex_shader.vert").c_str(), 
        (shaderPath + "fragment_shader.frag").c_str()
    ));
    
    skyboxShader = ShaderPtr(new Shader(
        (shaderPath + "skybox.vert").c_str(), 
        (shaderPath + "skybox.frag").c_str()
    ));
    
    lightShader = ShaderPtr(new Shader(
        (shaderPath + "vertex_shader.vert").c_str(), 
        (shaderPath + "fragment_shader.frag").c_str()
    ));
    
    butterflyShader = ShaderPtr(new Shader(
        (shaderPath + "butterfly.vert").c_str(),
        (shaderPath + "butterfly.frag").c_str()
    ));
}

void cleanupShaders()
{
    // Reset all shader pointers (automatically deletes if last reference)
    ourShader.reset();
    skyboxShader.reset();
    lightShader.reset();
    butterflyShader.reset();
    
    // Clear any OpenGL errors
    while(glGetError() != GL_NO_ERROR);
}
