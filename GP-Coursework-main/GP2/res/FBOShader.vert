#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform float time;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(aPos.x + sin(time) / 10, aPos.y + cos(time) / 10, 0.0, 1.0); 
    TexCoords = aTexCoords;
}  