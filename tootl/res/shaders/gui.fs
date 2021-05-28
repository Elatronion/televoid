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

bool dither(float opacity, float alpha_clip) {
  // Fancy dithered alpha stuff
  int x = int(gl_FragCoord.x) % 4;
  int y = int(gl_FragCoord.y) % 4;
  int index = x + y * 4;
  float limit = 0.0;

  if (x < 8) {
      if (index == 0) limit = 0.0625;
      if (index == 1) limit = 0.5625;
      if (index == 2) limit = 0.1875;
      if (index == 3) limit = 0.6875;
      if (index == 4) limit = 0.8125;
      if (index == 5) limit = 0.3125;
      if (index == 6) limit = 0.9375;
      if (index == 7) limit = 0.4375;
      if (index == 8) limit = 0.25;
      if (index == 9) limit = 0.75;
      if (index == 10) limit = 0.125;
      if (index == 11) limit = 0.625;
      if (index == 12) limit = 1.0;
      if (index == 13) limit = 0.5;
      if (index == 14) limit = 0.875;
      if (index == 15) limit = 0.375;
  }
  // Is this pixel below the opacity limit?
  return !(opacity < limit || opacity < alpha_clip);
}

void main()
{
    color = texture(material.diffuse, TexCoords) * material.color_multiplier;

    if(color.a == 0)
      discard;

    if(!dither(color.a, 0)) {
      discard;
    } else {
      color.rgb *= color.a;
      color.a = 1;
    }
}
