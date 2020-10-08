#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

layout(origin_upper_left) in vec4 gl_FragCoord;

uniform sampler2D ourTexture;

uniform int transparent;

void main() {
  FragColor = texture(ourTexture, TexCoord);
  if(FragColor.a == 0) discard;
  if(transparent == 1) {
    int grid_size = 2;
    int pixel_x = int(gl_FragCoord.x);
    int pixel_y = int(gl_FragCoord.y);
    if(pixel_x % grid_size == pixel_y % grid_size) discard;
  }
}
