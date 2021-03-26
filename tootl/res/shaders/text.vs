#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoords;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//uniform mat4 gui_projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos.xy, 0.0, 1.0);
    TexCoords = vec2(aTexCoord.x, 1.0 - aTexCoord.y);
}
