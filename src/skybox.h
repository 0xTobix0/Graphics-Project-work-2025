#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>
#include <string>

// Include OpenGL headers
#include "glad/gl.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Forward declaration of Shader class
class Shader;

class Skybox {
public:
    Skybox(const std::vector<std::string>& faces, Shader& shader);
    ~Skybox();
    
    void Draw(glm::mat4 view, glm::mat4 projection);
    
private:
    unsigned int VAO, VBO;
    unsigned int cubemapTexture;
    Shader& shader;
    
    unsigned int loadCubemap(const std::vector<std::string>& faces);
};

#endif // SKYBOX_H
