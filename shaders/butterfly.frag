#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// Material properties
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    bool useDiffuseMap;
    bool useSpecularMap;
    bool useNormalMap;
};

// Light properties
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Textures
uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;

// Material and light uniforms
uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main()
{
    // Discard fragments that are part of the base (Y < -0.1)
    if (FragPos.y < -0.1) {
        discard;
    }
    
    // Sample texture maps if available
    vec3 texDiffuse = material.useDiffuseMap ? 
        texture(diffuseMap, TexCoords).rgb : material.diffuse;
    
    vec3 texSpecular = material.useSpecularMap ? 
        texture(specularMap, TexCoords).rgb : material.specular;
    
    // Ambient lighting
    vec3 ambient = light.ambient * texDiffuse * material.ambient;
    
    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texDiffuse * material.diffuse;
    
    // Specular lighting
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texSpecular * material.specular;
    
    // Combine lighting components
    vec3 result = ambient + diffuse + specular;
    
    // Apply gamma correction
    float gamma = 2.2;
    result = pow(result, vec3(1.0/gamma));
    
    FragColor = vec4(result, 1.0);
}
