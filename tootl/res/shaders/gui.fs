#version 330 core
in vec2 TexCoords;
out vec4 color;

struct Material {
  sampler2D diffuse;
  sampler2D normal;
};

uniform Material material;
uniform vec4 gui_color;

void main()
{
    //color = vec4(1, 1, 1, 1);
    //vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    //color = textColor * sampled;
    color = texture(material.diffuse, TexCoords) * gui_color;
    if(color.a == 0)
    discard;
    //color = vec4(0, 0, 0, 0.25);
}
