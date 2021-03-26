
#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec3 ourColor; // output a color to the fragment shader
out vec2 TexCoord;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Sprite Sheet
uniform bool is_spritesheet;
uniform int frame_resolution_x;
uniform int frame_resolution_y;
uniform int image_resx;
uniform int image_resy;
uniform int frame_x;
uniform int frame_y;

uniform float runtime;

void DoSpriteSheetMath() {
  float size_x = float(frame_resolution_x)/float(image_resx);
  float size_y = float(frame_resolution_y)/float(image_resy);

  float pos_x = float(frame_x)*float(size_x);
  float pos_y = float(frame_y)*float(size_y);

  TexCoord = vec2(pos_x + TexCoord.x*size_x, pos_y + TexCoord.y*size_y);
}

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);

    if(is_spritesheet)
      DoSpriteSheetMath();

    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = aNormal;
}
