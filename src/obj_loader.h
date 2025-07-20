#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include "shader.h"

// STB Image wrapper
#include "stb_image_wrapper.h"

// Simple structure to hold material properties
struct Material {
    std::string name;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    GLuint diffuseMap;
    GLuint specularMap;
    GLuint normalMap;
    
    Material() : 
        name(""),
        ambient(0.1f, 0.1f, 0.1f), 
        diffuse(0.7f, 0.7f, 0.7f), 
        specular(0.5f, 0.5f, 0.5f), 
        shininess(32.0f),
        diffuseMap(0),
        specularMap(0),
        normalMap(0) {}
};

// Structure to hold mesh data
struct Mesh {
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    size_t indexCount;
    int materialIndex;
    
    Mesh() : vao(0), vbo(0), ebo(0), indexCount(0), materialIndex(-1) {}
};

class OBJLoader {
private:
    Shader& shader;
    std::vector<Mesh> meshes;
    std::vector<Material> materials;
    std::string baseDir; // Directory containing the OBJ file
    bool hasTextures = false;  // Add this line
    
public:
    OBJLoader(Shader& shader);
    ~OBJLoader();
    
    bool LoadModel(const std::string& objPath);
    void Draw(Shader& shader);
    
    // Helper methods
    bool LoadMaterials(const std::string& mtlPath);
    GLuint LoadTexture(const std::string& path);
    void ProcessMesh(const std::vector<glm::vec3>& vertices,
                    const std::vector<glm::vec3>& normals,
                    const std::vector<glm::vec2>& texCoords,
                    const std::vector<unsigned int>& indices,
                    int materialIndex);
};

#endif // OBJ_LOADER_H
