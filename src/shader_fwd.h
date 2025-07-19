#pragma once

#include <memory>

// Forward declaration of Shader class to avoid circular includes
class Shader;

// Alias for Shader unique_ptr
using ShaderPtr = std::unique_ptr<Shader>;
