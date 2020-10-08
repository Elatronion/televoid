#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(position.x*2, position.y*2, 0.0, 1.0);
    TexCoords = texCoords;
}
