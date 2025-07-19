#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float leftWingAngle;
uniform float rightWingAngle;

void main()
{
    vec4 pos = vec4(aPos, 1.0);
    
    // Apply wing animation
    if (aPos.x < -0.1) {  // Left wing
        float angle = radians(90.0) * leftWingAngle;
        mat4 rotation = mat4(1.0);
        rotation[1] = vec4(0.0, cos(angle), -sin(angle), 0.0);
        rotation[2] = vec4(0.0, sin(angle), cos(angle), 0.0);
        pos = rotation * pos;
    } else if (aPos.x > 0.1) {  // Right wing
        float angle = radians(90.0) * rightWingAngle;
        mat4 rotation = mat4(1.0);
        rotation[1] = vec4(0.0, cos(angle), -sin(angle), 0.0);
        rotation[2] = vec4(0.0, sin(angle), cos(angle), 0.0);
        pos = rotation * pos;
    }
    
    gl_Position = projection * view * model * pos;
    ourColor = aColor;
}
