#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

in float time;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(aPos.x + sin(time), aPos.y + cos(time), 0.0, 1.0); 
    TexCoords = aTexCoords;
}  