#include "obj_loader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>  // For std::setprecision
#include <chrono>   // For timing
#include <cstring>  // For strerror
#include <unordered_map>
#include <unistd.h>  // For getcwd
#include <map>
#include <string>
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/gl.h>
#include "shader.h"

// STB image wrapper
#include "stb_image_wrapper.h"

// Simple wrapper for STB image loading
namespace {
    void SetSTBIFlipVertically(bool flip) {
        stbi_set_flip_vertically_on_load(flip ? 1 : 0);
    }
    
    const char* GetSTBILoadError() {
        return stbi_get_error_message();
    }
}

// Helper function to split a string by a delimiter
static std::vector<std::string> split(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

OBJLoader::OBJLoader(Shader& shader) : shader(shader) {
    // Initialize with default material
    Material defaultMat;
    defaultMat.name = "default";
    defaultMat.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    defaultMat.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    defaultMat.specular = glm::vec3(0.5f, 0.5f, 0.5f);
    defaultMat.shininess = 32.0f;
    
    // Store default material in the materials vector
    materials.push_back(defaultMat);
    
    std::cout << "OBJLoader initialized with shader ID: " << shader.ID << std::endl;
}

OBJLoader::~OBJLoader() {
    // Clean up OpenGL resources
    for (auto& mesh : meshes) {
        glDeleteVertexArrays(1, &mesh.vao);
        glDeleteBuffers(1, &mesh.vbo);
        glDeleteBuffers(1, &mesh.ebo);
    }
    
    // Delete textures
    for (auto& material : materials) {
        if (material.diffuseMap) glDeleteTextures(1, &material.diffuseMap);
        if (material.specularMap) glDeleteTextures(1, &material.specularMap);
        if (material.normalMap) glDeleteTextures(1, &material.normalMap);
    }
}

bool OBJLoader::LoadModel(const std::string& path) {
    std::cout << "Loading OBJ model: " << path << std::endl;
    
    // Get current working directory for debugging
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        std::cout << "Current working directory: " << cwd << std::endl;
    }
    
    // Clear any existing meshes
    meshes.clear();
    materials.clear();
    hasTextures = false;
    
    // Extract base directory from path
    size_t lastSlash = path.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        baseDir = path.substr(0, lastSlash + 1);
    } else {
        baseDir = "./";
    }
    std::cout << "Base directory for assets: " << baseDir << std::endl;
    
    // Try to open the file
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << path << std::endl;
        std::cerr << "Error: " << strerror(errno) << std::endl;
        
        // Try to find the file in the current directory
        std::string filename = path.substr(path.find_last_of("/\\") + 1);
    }
    
    std::string line;
    std::string currentMtl;
    
    // Reserve space for better performance with large files
    const size_t ESTIMATED_VERTICES = 50000;
    std::vector<glm::vec3> tempVertices;
    tempVertices.reserve(ESTIMATED_VERTICES);
    std::vector<glm::vec3> tempNormals;
    tempNormals.reserve(ESTIMATED_VERTICES);
    std::vector<glm::vec2> tempTexCoords;
    tempTexCoords.reserve(ESTIMATED_VERTICES);
    
    // Current mesh data
    std::vector<glm::vec3> vertices;
    vertices.reserve(ESTIMATED_VERTICES);
    std::vector<glm::vec3> normals;
    normals.reserve(ESTIMATED_VERTICES);
    std::vector<glm::vec2> texCoords;
    texCoords.reserve(ESTIMATED_VERTICES);
    std::vector<unsigned int> indices;
    indices.reserve(ESTIMATED_VERTICES * 3);
    
    size_t lineNumber = 0;
    
    // Get file size for progress reporting
    file.seekg(0, std::ios::end);
    size_t fileSize = static_cast<size_t>(file.tellg());
    file.seekg(0, std::ios::beg);
    
    std::cout << "\n=== Loading OBJ file ===" << std::endl;
    std::cout << "File: " << path << std::endl;
    std::cout << "Size: ";
    if (fileSize > 1024 * 1024) {
        std::cout << (fileSize / (1024 * 1024)) << " MB";
    } else {
        std::cout << (fileSize / 1024) << " KB";
    }
    std::cout << std::endl;
    
    std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
    size_t lastPrintPos = 0;
    const size_t PRINT_INTERVAL = 5 * 1024 * 1024; // Print every 5MB
    
    while (std::getline(file, line)) {
        lineNumber++;
        
        // Show progress
        std::streampos currentPos = file.tellg();
        size_t currentPosSize = static_cast<size_t>(currentPos);
        
        if (currentPosSize - lastPrintPos >= PRINT_INTERVAL || currentPosSize >= fileSize) {
            float progress = (static_cast<float>(currentPosSize) / fileSize) * 100.0f;
            std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
            float elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                now - startTime).count() / 1000.0f;
            float speed = (currentPosSize / (1024.0f * 1024.0f)) / (elapsed > 0 ? elapsed : 1);
            
            std::cout << "\rProgress: [";
            int pos = static_cast<int>(progress / 2);
            for (int i = 0; i < 50; ++i) {
                if (i < pos) std::cout << "=";
                else if (i == pos) std::cout << ">";
                else std::cout << " ";
            }
            std::cout << "] " << std::fixed << std::setprecision(1) << progress 
                     << "% (" << speed << " MB/s)" << std::flush;
            
            lastPrintPos = currentPosSize;
        }
        
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') continue;
        
        std::istringstream iss(line);
        std::string prefix;
        if (!(iss >> prefix)) continue;
        
        if (lineNumber % 1000 == 0) {
            std::cout << "Processing line " << lineNumber << ": " << prefix << "..." << std::endl;
            std::cout << "  Vertices: " << tempVertices.size() 
                     << ", TexCoords: " << tempTexCoords.size()
                     << ", Normals: " << tempNormals.size() << std::endl;
        }
        
        if (prefix == "v") {
            // Vertex position
            glm::vec3 vertex(0.0f, 0.0f, 0.0f);
            if (!(iss >> vertex.x)) continue;
            if (!(iss >> vertex.y)) continue;
            if (!(iss >> vertex.z)) continue;
            tempVertices.push_back(vertex);
        } else if (prefix == "vt") {
            // Texture coordinate
            glm::vec2 texCoord(0.0f, 0.0f);
            if (!(iss >> texCoord.s)) continue;
            if (!(iss >> texCoord.t)) continue;
            texCoord.t = 1.0f - texCoord.t; // Flip V coordinate
            tempTexCoords.push_back(texCoord);
        } else if (prefix == "vn") {
            // Vertex normal
            glm::vec3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            tempNormals.push_back(normal);
        } else if (prefix == "f") {
            // Face
            std::vector<std::string> faceVertices;
            {
                // Extract face vertices more carefully
                std::string faceData = line.substr(1); // Skip 'f' and any whitespace
                std::istringstream faceStream(faceData);
                std::string vertexDef;
                while (faceStream >> vertexDef) {
                    if (!vertexDef.empty()) {
                        faceVertices.push_back(vertexDef);
                    }
                }
            }
            
            // For now, just handle triangles
            if (faceVertices.size() >= 3) {
                // For each vertex in the face
                for (size_t i = 0; i < faceVertices.size(); i++) {
                    std::istringstream vertexIss(faceVertices[i]);
                    std::vector<std::string> indicesStr;
                    std::string indexStr;
                    
                    // Split by '/' but handle empty segments properly
                    while (std::getline(vertexIss, indexStr, '/')) {
                        // Only add non-empty segments
                        if (!indexStr.empty()) {
                            indicesStr.push_back(indexStr);
                        } else {
                            // Add empty string to maintain position
                            indicesStr.push_back("");
                        }
                    }
                    
                    // Get vertex position index (required)
                    int vIdx = -1;
                    if (!indicesStr.empty() && !indicesStr[0].empty()) {
                        try {
                            vIdx = std::stoi(indicesStr[0]);
                            if (vIdx > 0) {
                                vIdx--; // Convert to 0-based index
                            } else if (vIdx < 0) {
                                // Handle negative indices (relative to end of array)
                                vIdx = tempVertices.size() + vIdx;
                                if (vIdx < 0) {
                                    std::cerr << "Error: Invalid negative vertex index at line " << lineNumber 
                                             << ": " << indicesStr[0] << std::endl;
                                    continue;
                                }
                            } else {
                                std::cerr << "Error: Invalid vertex index (0) at line " << lineNumber 
                                         << ": " << indicesStr[0] << std::endl;
                                continue;
                            }
                        } catch (const std::invalid_argument& e) {
                            std::cerr << "Warning: Invalid vertex index format at line " << lineNumber 
                                     << ": '" << indicesStr[0] << "' - skipping face" << std::endl;
                            continue;
                        } catch (const std::out_of_range& e) {
                            std::cerr << "Warning: Vertex index out of range at line " << lineNumber 
                                     << ": " << indicesStr[0] << " - skipping face" << std::endl;
                            continue;
                        }
                    } else {
                        std::cerr << "Warning: Face vertex missing position index at line " << lineNumber 
                                 << " - skipping face" << std::endl;
                        continue;
                    }
                    
                    // Get texture coordinate index (optional)
                    int vtIdx = -1;
                    if (indicesStr.size() > 1 && !indicesStr[1].empty()) {
                        try {
                            vtIdx = std::stoi(indicesStr[1]);
                            if (vtIdx > 0) {
                                vtIdx--; // Convert to 0-based index
                            } else if (vtIdx < 0) {
                                // Handle negative indices (relative to end of array)
                                vtIdx = tempTexCoords.size() + vtIdx;
                                if (vtIdx < 0) {
                                    std::cerr << "Warning: Invalid negative texture coordinate index at line " 
                                             << lineNumber << ": " << indicesStr[1] << " - using default value" << std::endl;
                                    vtIdx = -1;
                                }
                            } else {
                                std::cerr << "Warning: Invalid texture coordinate index (0) at line " 
                                         << lineNumber << ": " << indicesStr[1] << " - using default value" << std::endl;
                                vtIdx = -1;
                            }
                        } catch (const std::invalid_argument& e) {
                            std::cerr << "Warning: Invalid texture coordinate index format at line " 
                                     << lineNumber << ": '" << indicesStr[1] << "' - using default value" << std::endl;
                        } catch (const std::out_of_range& e) {
                            std::cerr << "Warning: Texture coordinate index out of range at line " 
                                     << lineNumber << ": " << indicesStr[1] << " - using default value" << std::endl;
                        }
                    }
                    
                    // Get normal index (optional)
                    int vnIdx = -1;
                    if (indicesStr.size() > 2 && !indicesStr[2].empty()) {
                        try {
                            vnIdx = std::stoi(indicesStr[2]);
                            if (vnIdx > 0) {
                                vnIdx--; // Convert to 0-based index
                            } else if (vnIdx < 0) {
                                // Handle negative indices (relative to end of array)
                                vnIdx = tempNormals.size() + vnIdx;
                                if (vnIdx < 0) {
                                    std::cerr << "Warning: Invalid negative normal index at line " 
                                             << lineNumber << ": " << indicesStr[2] << " - using default value" << std::endl;
                                    vnIdx = -1;
                                }
                            } else {
                                std::cerr << "Warning: Invalid normal index (0) at line " 
                                         << lineNumber << ": " << indicesStr[2] << " - using default value" << std::endl;
                                vnIdx = -1;
                            }
                        } catch (const std::invalid_argument& e) {
                            std::cerr << "Warning: Invalid normal index format at line " 
                                     << lineNumber << ": '" << indicesStr[2] << "' - using default value" << std::endl;
                        } catch (const std::out_of_range& e) {
                            std::cerr << "Warning: Normal index out of range at line " 
                                     << lineNumber << ": " << indicesStr[2] << " - using default value" << std::endl;
                        }
                    }
                    
                    // Check if vertex is valid
                    if (vIdx < 0 || vIdx >= tempVertices.size()) {
                        std::cerr << "Error: Invalid vertex index " << (vIdx+1) << " at line " << lineNumber << std::endl;
                        continue;
                    }
                    
                    // Add vertex position
                    vertices.push_back(tempVertices[vIdx]);
                    
                    // Add texture coordinate if available, otherwise use default
                    if (vtIdx >= 0 && static_cast<size_t>(vtIdx) < tempTexCoords.size()) {
                        texCoords.push_back(tempTexCoords[vtIdx]);
                    } else {
                        texCoords.push_back(glm::vec2(0.0f, 0.0f));
                    }
                    
                    // Add normal if available, otherwise use default
                    if (vnIdx >= 0 && static_cast<size_t>(vnIdx) < tempNormals.size()) {
                        normals.push_back(tempNormals[vnIdx]);
                    } else {
                        // Calculate face normal if not provided
                        if (i >= 2 && vertices.size() >= 3) {
                            glm::vec3 v0 = vertices[vertices.size()-3];
                            glm::vec3 v1 = vertices[vertices.size()-2];
                            glm::vec3 v2 = vertices[vertices.size()-1];
                            glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
                            normals.push_back(normal);
                        } else {
                            normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
                        }
                    }
                    
                    // Check if any vertex in this face is part of the base (Y < -0.1)
                    bool isBase = false;
                    for (int i = 0; i < faceVertices.size(); i++) {
                        std::istringstream vertexIss(faceVertices[i]);
                        std::vector<std::string> indicesStr;
                        std::string indexStr;
                        while (std::getline(vertexIss, indexStr, '/')) {
                            indicesStr.push_back(indexStr);
                        }
                        int idx = std::stoi(indicesStr[0]) - 1;
                        if (idx >= 0 && idx < tempVertices.size() && tempVertices[idx].y < -0.1f) {
                            isBase = true;
                            break;
                        }
                    }
                    
                    // Skip faces that are part of the base
                    if (isBase) {
                        continue;
                    }
                    
                    // Add index
                    indices.push_back(static_cast<unsigned int>(vertices.size() - 1));
                }
                
                // If this is a quad, triangulate it
                if (faceVertices.size() > 3) {
                    // Add two more triangles to form a quad
                    size_t base = vertices.size() - faceVertices.size();
                    indices.push_back(static_cast<unsigned int>(base));
                    indices.push_back(static_cast<unsigned int>(base + 2));
                    indices.push_back(static_cast<unsigned int>(base + 3));
                }
            }
        } else if (prefix == "mtllib") {
            // Load material library
            std::string mtlFile;
            iss >> mtlFile;
            std::string fullMtlPath = baseDir + mtlFile;
            std::cout << "Loading material library: " << fullMtlPath << std::endl;
            if (!LoadMaterials(fullMtlPath)) {
                std::cerr << "Failed to load material library: " << fullMtlPath << std::endl;
            } else {
                std::cout << "Successfully loaded " << materials.size() << " materials" << std::endl;
            }
        } else if (prefix == "usemtl") {
            // If we have a mesh in progress, finalize it
            if (!vertices.empty()) {
                std::cout << "Processing mesh with " << vertices.size() << " vertices, " 
                          << indices.size() << " indices, material: " << currentMtl << std::endl;
                ProcessMesh(vertices, normals, texCoords, indices, 
                          currentMtl.empty() ? -1 : 0); // Simple material handling for now
                vertices.clear();
                normals.clear();
                texCoords.clear();
                indices.clear();
            }
            iss >> currentMtl;
        }
    }
    
    // Process any remaining vertices
    if (!vertices.empty()) {
        std::cout << "Processing final mesh with " << vertices.size() << " vertices, " 
                  << indices.size() << " indices, material: " << currentMtl << std::endl;
        ProcessMesh(vertices, normals, texCoords, indices, 
                   currentMtl.empty() ? -1 : 0);
    }
    
    if (meshes.empty()) {
        std::cerr << "ERROR: No meshes were created from the OBJ file!" << std::endl;
        return false;
    }
    
    if (meshes.empty()) {
        std::cerr << "WARNING: No vertices were loaded from the OBJ file!" << std::endl;
        std::cerr << "  Total vertices in file: " << tempVertices.size() << std::endl;
        std::cerr << "  Total texture coordinates: " << tempTexCoords.size() << std::endl;
        std::cerr << "  Total normals: " << tempNormals.size() << std::endl;
        std::cerr << "  Total indices: " << indices.size() << " (" << (indices.size() / 3) << " triangles)" << std::endl;
        std::cerr << "  Current working directory: " << (getcwd(nullptr, 0) ?: "unknown") << std::endl;
        
        // Print first few vertices for debugging
        std::cout << "\nFirst 5 vertices:" << std::endl;
        for (size_t i = 0; i < std::min(tempVertices.size(), (size_t)5); ++i) {
            std::cout << "  v " << tempVertices[i].x << " " 
                     << tempVertices[i].y << " " << tempVertices[i].z << std::endl;
        }
        
        // Print first few texture coordinates for debugging
        std::cout << "\nFirst 5 texture coordinates:" << std::endl;
        for (size_t i = 0; i < std::min(tempTexCoords.size(), (size_t)5); ++i) {
            std::cout << "  vt " << tempTexCoords[i].s << " " << tempTexCoords[i].t << std::endl;
        }
        
        // Print first few normals for debugging
        std::cout << "\nFirst 5 normals:" << std::endl;
        for (size_t i = 0; i < std::min(tempNormals.size(), (size_t)5); ++i) {
            std::cout << "  vn " << tempNormals[i].x << " " 
                     << tempNormals[i].y << " " << tempNormals[i].z << std::endl;
        }
        
        // Print first few faces for debugging
        std::cout << "\nFirst 5 faces:" << std::endl;
        for (size_t i = 0; i < std::min(indices.size()/3, (size_t)5); ++i) {
            std::cout << "  f " << indices[i*3] << "/" << indices[i*3+1] << "/" << indices[i*3+2] 
                     << std::endl;
        }
        
        // Print first few lines of the file for debugging
        std::ifstream file(path);
        if (file.is_open()) {
            std::cout << "\nFirst 10 lines of OBJ file:" << std::endl;
            std::string line;
            int lineCount = 0;
            while (std::getline(file, line) && lineCount < 10) {
                std::cout << "  " << line << std::endl;
                lineCount++;
            }
        } else {
            std::cerr << "Could not open file to read first 10 lines" << std::endl;
        }
        
        return false;
    }
    
    std::cout << "Successfully loaded model with " << meshes.size() << " meshes and " 
              << materials.size() << " materials" << std::endl;
    return true;
}

bool OBJLoader::LoadMaterials(const std::string& mtlPath) {
    std::ifstream file(mtlPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open MTL file: " << mtlPath << std::endl;
        return false;
    }
    
    Material currentMtl;
    std::string line;
    bool firstMaterial = true;
    
    // Map material names to texture types for the butterfly model
    std::map<std::string, std::string> materialToTexture = {
        {"wire_154215229", "Alas_Corona_Beauty.jpg"},  // Wing material
        {"wire_184007009", "Venas_Corona_Beauty.jpg"},  // Vein material
        {"wire_255255000", "Cuerpo_Corona_Beauty.jpg"}, // Body material
        {"wire_135059008", "Suelo_Corona_Beauty.jpg"},  // Ground material (not used)
        {"wire_042116168", "Alas_Corona_ReflectColor.jpg"}, // Wing reflection
        {"wire_000255000", "Cuerpo_Corona_ReflectColor.jpg"} // Body reflection
    };
    
    // Print base directory for debugging
    std::cout << "Base directory for textures: " << baseDir << std::endl;
    
    while (std::getline(file, line)) {
        // Skip comments and empty lines
        if (line.empty() || line[0] == '#') continue;
        
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        
        if (prefix == "newmtl") {
            if (!firstMaterial) {
                materials.push_back(currentMtl);
            } else {
                firstMaterial = false;
            }
            currentMtl = Material();
            iss >> currentMtl.name;
            
            // Assign appropriate texture based on material name
            auto it = materialToTexture.find(currentMtl.name);
            if (it != materialToTexture.end()) {
                // Look for textures in the same directory as the OBJ file
                std::string texPath = baseDir + it->second;
                std::cout << "Loading texture: " << texPath << std::endl;
                currentMtl.diffuseMap = LoadTexture(texPath);
                
                // For reflective parts, also load the reflection map
                if (it->first == "wire_042116168" || it->first == "wire_000255000") {
                    std::string reflectPath = baseDir + it->second;
                    std::cout << "Loading reflection map: " << reflectPath << std::endl;
                    currentMtl.specularMap = LoadTexture(reflectPath);
                }
            }
        } else if (prefix == "Ka") {
            // Ambient color
            iss >> currentMtl.ambient.r >> currentMtl.ambient.g >> currentMtl.ambient.b;
        } else if (prefix == "Kd") {
            // Diffuse color
            iss >> currentMtl.diffuse.r >> currentMtl.diffuse.g >> currentMtl.diffuse.b;
        } else if (prefix == "Ks") {
            // Specular color
            iss >> currentMtl.specular.r >> currentMtl.specular.g >> currentMtl.specular.b;
        } else if (prefix == "Ns") {
            // Shininess
            iss >> currentMtl.shininess;
        }
    }
    
    // Add the last material
    if (!currentMtl.name.empty()) {
        materials.push_back(currentMtl);
    }
    
    return !materials.empty();
}

GLuint OBJLoader::LoadTexture(const std::string& path) {
    // Check if file exists and is readable
    std::ifstream file(path, std::ios::binary);
    if (!file.good()) {
        std::cerr << "ERROR: Texture file does not exist or is not readable: " << path << std::endl;
        return 0;
    }
    file.close();
    
    // Set STBI to flip textures vertically (OpenGL expects textures to start from bottom-left)
    SetSTBIFlipVertically(true);
    
    // Load image data using stb_image
    int width, height, channels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    
    if (!data) {
        std::cerr << "ERROR: Failed to load texture: " << path << std::endl;
        const char* error = GetSTBILoadError();
        if (error) {
            std::cerr << "  STBI Error: " << error << std::endl;
        }
        return 0;
    }
    
    std::cout << "Successfully loaded texture: " << path << std::endl;
    std::cout << "  Dimensions: " << width << "x" << height << ", Channels: " << channels << std::endl;
    
    // Generate and bind texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Determine the format based on number of channels
    GLenum format = GL_RGB;
    if (channels == 4) {
        format = GL_RGBA;
    } else if (channels == 1) {
        format = GL_RED;
    } else if (channels != 3) {
        std::cerr << "WARNING: Unsupported number of channels (" << channels << ") in texture: " << path << std::endl;
    }
    
    // Upload texture data to GPU
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    
    // Generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Check for OpenGL errors
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error after loading texture " << path << ": " << error << std::endl;
    }
    
    // Free the image data
    stbi_image_free(data);
    
    std::cout << "  Texture ID: " << textureID << " (GL error: " << error << ")" << std::endl;
    
    return textureID;
}

void OBJLoader::ProcessMesh(const std::vector<glm::vec3>& vertices,
                           const std::vector<glm::vec3>& normals,
                           const std::vector<glm::vec2>& texCoords,
                           const std::vector<unsigned int>& indices,
                           int materialIndex) {
    if (vertices.empty()) return;
    
    // Create and bind VAO
    Mesh mesh;
    mesh.materialIndex = materialIndex;
    mesh.indexCount = indices.size();
    
    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);
    
    // Create and bind VBO for vertices, normals, and texture coordinates
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    
    // Interleave vertex data (position, normal, texCoord)
    std::vector<float> vertexData;
    std::vector<unsigned int> filteredIndices;
    std::unordered_map<unsigned int, unsigned int> indexMap; // Maps old indices to new ones
    
    // First pass: filter out vertices that are part of the base (Y < -0.1)
    for (size_t i = 0; i < indices.size(); i += 3) {
        // Check if any vertex in this triangle is part of the base
        bool skipTriangle = false;
        for (int j = 0; j < 3; j++) {
            unsigned int idx = indices[i + j];
            if (vertices[idx].y < -0.1f) {
                skipTriangle = true;
                break;
            }
        }
        
        if (!skipTriangle) {
            // Add all three indices of the triangle
            for (int j = 0; j < 3; j++) {
                unsigned int oldIdx = indices[i + j];
                
                // If we haven't seen this index before, add it to the vertex data
                if (indexMap.find(oldIdx) == indexMap.end()) {
                    // Position
                    vertexData.push_back(vertices[oldIdx].x);
                    vertexData.push_back(vertices[oldIdx].y);
                    vertexData.push_back(vertices[oldIdx].z);
                    
                    // Normal
                    vertexData.push_back(normals[oldIdx].x);
                    vertexData.push_back(normals[oldIdx].y);
                    vertexData.push_back(normals[oldIdx].z);
                    
                    // Texture coordinates
                    vertexData.push_back(texCoords[oldIdx].s);
                    vertexData.push_back(texCoords[oldIdx].t);
                    
                    // Map the old index to the new one
                    indexMap[oldIdx] = filteredIndices.size();
                }
                
                // Add the new index to the filtered indices
                filteredIndices.push_back(indexMap[oldIdx]);
            }
        }
    }
    
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);
    
    // Set up vertex attributes
    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    
    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    
    // Texture coordinates
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    
    // Create and bind EBO with filtered indices
    glGenBuffers(1, &mesh.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, filteredIndices.size() * sizeof(unsigned int), filteredIndices.data(), GL_STATIC_DRAW);
    
    // Update the index count
    mesh.indexCount = filteredIndices.size();
    
    // Unbind VAO first, then VBO and EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    meshes.push_back(mesh);
}

void OBJLoader::Draw(Shader& shader) {
    if (meshes.empty()) {
        std::cerr << "OBJLoader::Draw: No meshes to draw!" << std::endl;
        return;
    }
    
    // Set shader uniforms for lighting
    shader.use();
    shader.setVec3("viewPos", glm::vec3(0.0f, 0.0f, 3.0f));
    shader.setVec3("light.position", glm::vec3(1.2f, 1.0f, 2.0f));
    shader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    shader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
    shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    
    // Draw all meshes
    for (const auto& mesh : meshes) {
        // Set material properties if material exists
        if (mesh.materialIndex >= 0 && mesh.materialIndex < materials.size()) {
            const auto& mat = materials[mesh.materialIndex];
            shader.setVec3("material.ambient", mat.ambient);
            shader.setVec3("material.diffuse", mat.diffuse);
            shader.setVec3("material.specular", mat.specular);
            shader.setFloat("material.shininess", mat.shininess);
            
            // Bind diffuse map
            if (mat.diffuseMap > 0) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, mat.diffuseMap);
                shader.setInt("material.diffuseMap", 0);
                shader.setBool("material.hasDiffuseMap", true);
            } else {
                shader.setBool("material.hasDiffuseMap", false);
            }
            
            // Bind specular map
            if (mat.specularMap > 0) {
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, mat.specularMap);
                shader.setInt("material.specularMap", 1);
                shader.setBool("material.hasSpecularMap", true);
            } else {
                shader.setBool("material.hasSpecularMap", false);
            }
            
            // Bind normal map
            if (mat.normalMap > 0) {
                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, mat.normalMap);
                shader.setInt("material.normalMap", 2);
                shader.setBool("material.hasNormalMap", true);
            } else {
                shader.setBool("material.hasNormalMap", false);
            }
        } else {
            // Default material
            shader.setVec3("material.ambient", 0.2f, 0.2f, 0.2f);
            shader.setVec3("material.diffuse", 0.8f, 0.8f, 0.8f);
            shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
            shader.setFloat("material.shininess", 32.0f);
            shader.setBool("material.hasDiffuseMap", false);
            shader.setBool("material.hasSpecularMap", false);
            shader.setBool("material.hasNormalMap", false);
        }
        
        // Draw mesh
        glBindVertexArray(mesh.vao);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.indexCount), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        // Reset texture units
        glActiveTexture(GL_TEXTURE0);
    }
    
    // Set up shader
    shader.use();
    std::cout << "OBJLoader: Drawing " << meshes.size() << " meshes with shader ID: " << shader.ID << std::endl;
    
    // Draw all meshes
    for (const auto& mesh : meshes) {
        // Set material properties if material exists
        if (mesh.materialIndex >= 0 && mesh.materialIndex < static_cast<int>(materials.size())) {
            const auto& material = materials[mesh.materialIndex];
            
            // Set material properties
            shader.setVec3("material.ambient", material.ambient);
            shader.setVec3("material.diffuse", material.diffuse);
            shader.setVec3("material.specular", material.specular);
            shader.setFloat("material.shininess", material.shininess);
            
            // Enable/disable texture usage flags
            shader.setBool("material.useDiffuseMap", material.diffuseMap != 0);
            shader.setBool("material.useSpecularMap", material.specularMap != 0);
            shader.setBool("material.useNormalMap", material.normalMap != 0);
            
            // Bind diffuse map
            if (material.diffuseMap) {
                glActiveTexture(GL_TEXTURE0);
                shader.setInt("material.diffuseMap", 0);
                glBindTexture(GL_TEXTURE_2D, material.diffuseMap);
            }
            
            // Bind specular map
            if (material.specularMap) {
                glActiveTexture(GL_TEXTURE1);
                shader.setInt("material.specularMap", 1);
                glBindTexture(GL_TEXTURE_2D, material.specularMap);
            }
            
            // Bind normal map (not used for butterfly, but keeping for completeness)
            if (material.normalMap) {
                glActiveTexture(GL_TEXTURE2);
                shader.setInt("material.normalMap", 2);
                glBindTexture(GL_TEXTURE_2D, material.normalMap);
            }
        } else {
            // Default material properties if no material is assigned
            shader.setVec3("material.ambient", 0.2f, 0.2f, 0.2f);
            shader.setVec3("material.diffuse", 0.8f, 0.8f, 0.8f);
            shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
            shader.setFloat("material.shininess", 32.0f);
            shader.setBool("material.useDiffuseMap", false);
            shader.setBool("material.useSpecularMap", false);
            shader.setBool("material.useNormalMap", false);
        }
        
        // Draw mesh
        glBindVertexArray(mesh.vao);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.indexCount), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        // Always unbind textures to prevent accidental reuse
        for (GLuint i = 0; i < 3; ++i) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
}
