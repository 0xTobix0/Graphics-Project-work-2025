#version 330 core

// Input vertex attributes
layout (location = 0) in vec3 aPos;      // Vertex position

// Output to fragment shader
out vec3 FragColor;

// Uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 instanceColor;

void main() {
    // Simple transformation
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    
    // Pass color to fragment shader
    FragColor = instanceColor;
}
