#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform vec2 u_resolution;

uniform sampler2D screenTexture;

uniform bool red_splash;
uniform bool blue_splash;

uniform float letterbox_percentage;

float easeinout(float a, float b, float t) {
  float diff = b - a;
  return (pow(t, 2)/(pow(t, 2) + pow(1 - t, 2))) * diff + a;
}

float lerp(float a, float b, float f) {
  return a + f * (b - a);
}

float random (vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}

vec4 vignette(vec4 color, float radius, float softness)
{
    vec2 position = (gl_FragCoord.xy / u_resolution) - vec2(0.5);
    float dist = length(position * vec2(u_resolution.x/u_resolution.y, 1.0));

    //float radius = 0.5;
    //float softness = 0.02;
    float vignette = smoothstep(radius, radius - softness, dist);

    color.rgb = color.rgb - (1.0 - vignette);

    return color;
}

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

  if(TexCoords.y < lerp(0, 0.5, letterbox_percentage) || TexCoords.y > lerp(1, 0.5, letterbox_percentage))
    color = vec4(0, 0, 0, 1);

  // Experiments
  // Random
  /*
  vec2 st = gl_FragCoord.xy/u_resolution.xy;
  st *= 100.0; // Scale the coordinate system by 10
  vec2 ipos = floor(st);  // get the integer coords
  vec2 fpos = fract(st);  // get the fractional coords

  // Assign a random value based on the integer coord
  float rnd = random( ipos );
  */
  //color = vec4(rnd, rnd, rnd, 1);

  // Vignette
  //color = color * vignette(vec4(1, 1, 1, 1), 1.5, 1);

  // Vignetted Noise
  /*
  vec4 vig = vignette(vec4(1, 1, 1, 1), 0.5, 0.2);
  float value = vig.x;
  float new_value = (-(value * 2 - 1) + 1) / 2;
  color = rnd * vec4(new_value, new_value, new_value, 1);
  */

  float value = vignette(vec4(1, 1, 1, 1), 1.15, 0.25).r;
  if(dither(value, 0)) {
    color = color * vec4(1, 1, 1, 1);
  } else {
    discard;
  }
}
