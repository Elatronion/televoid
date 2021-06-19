#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform vec2 u_resolution;
uniform float runtime;

uniform sampler2D screenTexture;

void main() {
	color = texture(screenTexture, TexCoords);
}
