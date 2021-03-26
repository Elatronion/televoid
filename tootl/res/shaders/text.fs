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

void main()
{

    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(material.diffuse, TexCoords).r);
    color = sampled * material.color_multiplier;
    if(sampled.a < 0.5)
      discard;

    color = material.color_multiplier * sampled;
}
