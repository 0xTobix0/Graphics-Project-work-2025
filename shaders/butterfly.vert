#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

// Material properties
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    bool hasDiffuseMap;
    bool hasSpecularMap;
    bool hasNormalMap;
};

uniform Material material;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

// Wing animation uniforms (keep for butterfly animation)
uniform float leftWingAngle;
uniform float rightWingAngle;

void main()
{
    // Apply model transformations
    vec4 worldPos = model * vec4(aPos, 1.0);
    
    // Wing animation temporarily disabled for debugging
    // if (aPos.x < -0.1) {  // Left wing
    //     float angle = radians(90.0) * leftWingAngle;
    //     mat4 rotation = mat4(1.0);
    //     rotation[1] = vec4(0.0, cos(angle), -sin(angle), 0.0);
    //     rotation[2] = vec4(0.0, sin(angle), cos(angle), 0.0);
    //     worldPos = rotation * worldPos;
    // } else if (aPos.x > 0.1) {  // Right wing
    //     float angle = radians(90.0) * rightWingAngle;
    //     mat4 rotation = mat4(1.0);
    //     rotation[1] = vec4(0.0, cos(angle), -sin(angle), 0.0);
    //     rotation[2] = vec4(0.0, sin(angle), cos(angle), 0.0);
    //     worldPos = rotation * worldPos;
    // }
    
    // Transform normal to world space using normal matrix
    Normal = normalize(normalMatrix * aNormal);
    
    // Pass data to fragment shader
    FragPos = vec3(worldPos);
    TexCoords = aTexCoords;
    
    // Final position
    gl_Position = projection * view * worldPos;
}
