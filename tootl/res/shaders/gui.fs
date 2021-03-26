#version 330 core
in vec2 TexCoords;
out vec4 color;

struct Material {
  sampler2D diffuse;
  sampler2D normal;
  bool lit;
  vec4 color_multiplier;
};

uniform Material material;
//uniform vec4 gui_color;

void main()
{
    color = texture(material.diffuse, TexCoords) * material.color_multiplier;

    if(color.a == 0)
      discard;
}
