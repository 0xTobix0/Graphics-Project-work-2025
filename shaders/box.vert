#version 330 core

// Input vertex attributes
layout (location = 0) in vec3 aPos;      // Vertex position
layout (location = 1) in vec3 aNormal;   // Vertex normal

// Output to fragment shader
out vec3 FragPos;
out vec3 Normal;

// Uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 instanceColor;
uniform bool isLightSource;  // Whether this box is a light source

void main() {
    // Transform position to world space
    vec4 worldPos = model * vec4(aPos, 1.0);
    
    // Pass position and normal to fragment shader
    FragPos = worldPos.xyz;
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    // Final position
    gl_Position = projection * view * worldPos;
}
