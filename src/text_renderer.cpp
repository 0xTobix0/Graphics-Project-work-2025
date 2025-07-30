#include "text_renderer.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "shader_manager.h"

TextRenderer::TextRenderer(unsigned int width, unsigned int height) 
    : Width(width), Height(height) {
    // Load text shader
    this->TextShader = GetShader("text");
    if (!this->TextShader) {
        std::cerr << "Failed to load text shader" << std::endl;
        return;
    }
    
    // Set up projection matrix for text rendering
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(Width), 
                                    0.0f, static_cast<float>(Height));
    this->TextShader->use();
    this->TextShader->setMat4("projection", projection);
    this->TextShader->setInt("text", 0);
    if (!this->TextShader) {
        std::cerr << "Failed to load text shader" << std::endl;
        return;
    }
    
    // Configure VAO/VBO for texture quads
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

bool TextRenderer::Load(std::string font, unsigned int fontSize) {
    std::cout << "Attempting to load font: " << font << " at size: " << fontSize << std::endl;
    
    // First clear the previously loaded Characters
    this->Characters.clear();
    
    // Initialize and load the FreeType library
    FT_Library ft;    
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return false;
    }
    
    // Load font as face
    FT_Face face;
    if (FT_New_Face(ft, font.c_str(), 0, &face)) {
        std::cerr << "ERROR::FREETYPE: Failed to load font: " << font << std::endl;
        std::cerr << "Current working directory: ";
        system("pwd");
        std::cerr << "Font file exists: " << (std::ifstream(font) ? "Yes" : "No") << std::endl;
        FT_Done_FreeType(ft);
        return false;
    }
    
    std::cout << "Successfully loaded font: " << font << std::endl;
    
    // Set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, fontSize);
    
    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    // Load first 128 characters of ASCII set
    for (GLubyte c = 0; c < 128; c++) {
        // Load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "ERROR::FREETYPE: Failed to load Glyph: " << c << std::endl;
            continue;
        }
        
        // Generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        // Now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }
    
    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    
    // Configure VAO/VBO for texture quads
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    return true;
}

void TextRenderer::RenderText(std::string text, float x, float y, float scale, glm::vec3 color) {
    static bool firstCall = true;
    if (firstCall) {
        std::cout << "First call to RenderText with text: " << text << std::endl;
        firstCall = false;
    }
    
    if (!this->TextShader) {
        std::cerr << "Text shader is null!" << std::endl;
        return;
    }
    
    if (this->Characters.empty()) {
        std::cerr << "No characters loaded in the font!" << std::endl;
        return;
    }
    
    // Activate corresponding render state
    this->TextShader->use();
    
    // Set up projection matrix for 2D rendering
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), 
                                     0.0f, static_cast<float>(this->Height));
    this->TextShader->setMat4("projection", projection);
    this->TextShader->setVec3("textColor", color);
    this->TextShader->setInt("text", 0);
    
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(this->VAO);
    
    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = Characters[*c];
        
        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
        
        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        
        // Update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h, 0.0f, 0.0f },            
            { xpos,     ypos,     0.0f, 1.0f },
            { xpos + w, ypos,     1.0f, 1.0f },
            
            { xpos,     ypos + h, 0.0f, 0.0f },
            { xpos + w, ypos,     1.0f, 1.0f },
            { xpos + w, ypos + h, 1.0f, 0.0f }           
        };
        
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        // Advance cursors for next glyph (advance is 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
    }
    
    // Clean up
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
