#pragma once

#include "shader_fwd.h"
#include <memory>
#include <string>

// Forward declaration
class Shader;

// Shader pointer type
typedef std::shared_ptr<Shader> ShaderPtr;

// Global shader pointers
extern ShaderPtr ourShader;
extern ShaderPtr skyboxShader;
extern ShaderPtr lightShader;
extern ShaderPtr butterflyShader;
extern ShaderPtr textShader;

// Function to initialize all shaders
void initShaders();

// Function to get a shader by name
ShaderPtr GetShader(const std::string& name);

// Function to clean up shaders
void cleanupShaders();

// Initialize shader manager
void InitializeShaderManager();
