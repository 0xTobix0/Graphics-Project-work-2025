#pragma once

#include "shader_fwd.h"

// Global shader pointers using ShaderPtr for automatic memory management
extern ShaderPtr ourShader;
extern ShaderPtr skyboxShader;
extern ShaderPtr lightShader;
extern ShaderPtr butterflyShader;

// Function to initialize all shaders
void initShaders();

// Function to clean up shaders
void cleanupShaders();
