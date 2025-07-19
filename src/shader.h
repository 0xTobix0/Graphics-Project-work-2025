#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    unsigned int ID;
    
    // Rule of Five
    Shader() = delete; // No default constructor
    Shader(const Shader&) = delete; // No copy constructor
    Shader& operator=(const Shader&) = delete; // No copy assignment
    Shader(Shader&& other) noexcept : ID(other.ID) { other.ID = 0; } // Move constructor
    Shader& operator=(Shader&& other) noexcept { // Move assignment
        if (this != &other) {
            if (ID) glDeleteProgram(ID);
            ID = other.ID;
            other.ID = 0;
        }
        return *this;
    }
    
    static void checkCompileErrors(unsigned int shader, const std::string& type) {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }

    Shader(const char* vertexPath, const char* fragmentPath) {
        // 1. Retrieve shader source code from file
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        
        try {
            // Open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            
            // Read file's buffer contents into streams
            std::stringstream vShaderStream, fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            
            // Close file handlers
            vShaderFile.close();
            fShaderFile.close();
            
            // Convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
            
            std::cout << "Successfully loaded shader files:\n" << vertexPath << "\n" << fragmentPath << std::endl;
            
        } catch (std::ifstream::failure& e) {
            std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
            return;
        }
        
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        
        // 2. Compile shaders
        unsigned int vertex, fragment;
        int success;
        char infoLog[512];
        
        // Vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        // Check for vertex shader compile errors
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        } else {
            std::cout << "Vertex shader compiled successfully" << std::endl;
        }
        
        // Fragment shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        // Check for fragment shader compile errors
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        } else {
            std::cout << "Fragment shader compiled successfully" << std::endl;
        }
        
        // Shader program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        
        // Check for linking errors
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        } else {
            std::cout << "Shader program linked successfully" << std::endl;
        }
        
        // Delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        
        // Validate program
        glValidateProgram(ID);
        glGetProgramiv(ID, GL_VALIDATE_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::VALIDATION_FAILED\n" << infoLog << std::endl;
        } else {
            std::cout << "Shader program validated successfully" << std::endl;
        }
    }
    
    virtual ~Shader() {
        if (ID) {
            glDeleteProgram(ID);
            ID = 0;
        }
    }
    
    void use() {
        glUseProgram(ID);
    }
    
    void setBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    
    void setInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    
    void setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    
    void setVec2(const std::string &name, const glm::vec2 &value) const {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    
    void setVec2(const std::string &name, float x, float y) const {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    }
    
    void setVec3(const std::string &name, const glm::vec3 &value) const {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    
    void setVec3(const std::string &name, float x, float y, float z) const {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }
    
    void setMat4(const std::string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    
private:
    void checkCompileErrors(unsigned int shader, std::string type) {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "\n=== SHADER COMPILATION ERROR (" << type << ") ===\n" 
                          << infoLog << "\n==========================================\n" << std::endl;
                
                // Print shader source for debugging
                GLint length;
                glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &length);
                if (length > 0) {
                    char* source = new char[length];
                    glGetShaderSource(shader, length, NULL, source);
                    std::cout << "Shader source:\n" << source << "\n" << std::endl;
                    delete[] source;
                }
            } else {
                std::cout << "Shader compilation successful: " << type << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "\n=== PROGRAM LINKING ERROR ===\n" 
                          << infoLog << "\n=================================\n" << std::endl;
                
                // Print attached shaders
                GLint count;
                glGetProgramiv(shader, GL_ATTACHED_SHADERS, &count);
                if (count > 0) {
                    GLuint* shaders = new GLuint[count];
                    GLsizei actualCount;
                    glGetAttachedShaders(shader, count, &actualCount, shaders);
                    std::cout << "Attached shaders (" << actualCount << "): ";
                    for (int i = 0; i < actualCount; ++i) {
                        std::cout << shaders[i] << " ";
                    }
                    std::cout << "\n" << std::endl;
                    delete[] shaders;
                }
            } else {
                std::cout << "Program linking successful" << std::endl;
                
                // Validate program
                glValidateProgram(shader);
                GLint validateStatus;
                glGetProgramiv(shader, GL_VALIDATE_STATUS, &validateStatus);
                if (validateStatus == GL_FALSE) {
                    glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                    std::cout << "\n=== PROGRAM VALIDATION ERROR ===\n" 
                              << infoLog << "\n==================================\n" << std::endl;
                } else {
                    std::cout << "Program validation successful" << std::endl;
                }
            }
        }
    }
};
