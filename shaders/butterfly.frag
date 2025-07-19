#version 330 core
out vec4 FragColor;

in vec3 ourColor;

void main()
{
    // Simple coloring based on the vertex color
    FragColor = vec4(ourColor, 1.0);
    
    // Add a bit of specular highlight for a more interesting look
    float specular = pow(max(dot(vec3(0.0, 1.0, 0.0), vec3(0.0, 1.0, 0.0)), 0.0), 32.0);
    FragColor.rgb += vec3(0.3) * specular;
}
