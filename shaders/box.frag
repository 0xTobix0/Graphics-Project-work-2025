#version 330 core

// Input from vertex shader
in vec3 FragColor;

// Output color
out vec4 outColor;

void main() {
    // Simple solid color
    outColor = vec4(FragColor, 1.0);
}
