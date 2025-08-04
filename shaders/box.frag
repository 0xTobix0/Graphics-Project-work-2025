#version 330 core

// Input from vertex shader
in vec3 FragPos;
in vec3 Normal;

// Output color
out vec4 outColor;

// Uniforms
uniform vec3 instanceColor;
uniform bool isLightSource;
uniform vec3 lightPos;      // Position of the light source
uniform vec3 viewPos;       // Position of the camera

void main() {
    if (isLightSource) {
        // If this is a light source, just emit light color
        outColor = vec4(instanceColor, 1.0);
    } else {
        // Ambient lighting
        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0);
        
        // Diffuse lighting
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * vec3(1.0, 1.0, 0.9);  // Slightly yellow light
        
        // Specular lighting (simple version)
        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * vec3(1.0, 1.0, 1.0);
        
        // Combine lighting
        vec3 result = (ambient + diffuse + specular) * instanceColor;
        outColor = vec4(result, 1.0);
    }
}
