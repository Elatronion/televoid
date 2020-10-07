#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

layout(origin_upper_left) in vec4 gl_FragCoord;

uniform sampler2D ourTexture;

uniform int transparent;

void main() {
  FragColor = texture(ourTexture, TexCoord);
  if(FragColor.a == 0) discard;
}
