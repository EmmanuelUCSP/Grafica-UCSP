#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(aPos);
    Normal = vec3(aNormal);
    
    gl_Position = projection * view * aPos;
}