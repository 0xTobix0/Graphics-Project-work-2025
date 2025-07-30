#pragma once

#include <memory>

// Forward declaration of Shader class to avoid circular includes
class Shader;

// Alias for Shader shared_ptr
using ShaderPtr = std::shared_ptr<Shader>;
