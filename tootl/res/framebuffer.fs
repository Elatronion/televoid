#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D screenTexture;

uniform float screen_alpha;

void main() {
    color = texture(screenTexture, TexCoords) * screen_alpha;
    //color = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);
}
