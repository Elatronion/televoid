#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec4 textColor;

void main()
{
    //color = vec4(1, 1, 1, 1);
    color = texture(text, TexCoords);
    //color = textColor * sampled;
    //if(sampled.a < 0.5)
    //discard;
}
