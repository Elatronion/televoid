#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D screenTexture;

uniform bool red_splash;
uniform bool blue_splash;

uniform float letterbox_percentage;

float easeinout(float a, float b, float t) {
  float diff = b - a;
  return (pow(t, 2)/(pow(t, 2) + pow(1 - t, 2))) * diff + a;
}

void main() {
  if(red_splash) {
    color = texture(screenTexture, TexCoords);
    //color.r = 0.2;
    color.g = 0.2;
    color.b = 0.2;
  } else if(blue_splash) {
    color = texture(screenTexture, TexCoords);
    color.r = 0.2;
    color.g = 0.2;
    //color.b = 0.2;
  } else {
    color = texture(screenTexture, TexCoords);
  }

  if(TexCoords.y < easeinout(0, 0.5, letterbox_percentage) || TexCoords.y > easeinout(1, 0.5, letterbox_percentage))
    color = vec4(0, 0, 0, 1);
}
