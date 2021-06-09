#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform int paused;
uniform int fast_foward;

uniform vec2 u_resolution;
uniform float runtime;

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

/*
  Effect : Distorted TV effect
  Source : https://www.shadertoy.com/view/ldXGW4
*/
// change these values to 0.0 to turn off individual effects
float vertJerkOpt =     0.0;
float vertMovementOpt = 0.0;
float bottomStaticOpt = 0.0;
float scalinesOpt =     0.0;
float rgbOffsetOpt =    0.0;
float horzFuzzOpt =     0.0;

// Noise generation functions borrowed from:
// https://github.com/ashima/webgl-noise/blob/master/src/noise2D.glsl

vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec2 mod289(vec2 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 permute(vec3 x) {
  return mod289(((x*34.0)+1.0)*x);
}

float snoise(vec2 v)
  {
  const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                      0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
                     -0.577350269189626,  // -1.0 + 2.0 * C.x
                      0.024390243902439); // 1.0 / 41.0
// First corner
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);

// Other corners
  vec2 i1;
  //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
  //i1.y = 1.0 - i1.x;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  // x0 = x0 - 0.0 + 0.0 * C.xx ;
  // x1 = x0 - i1 + 1.0 * C.xx ;
  // x2 = x0 - 1.0 + 2.0 * C.xx ;
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;

// Permutations
  i = mod289(i); // Avoid truncation effects in permutation
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
		+ i.x + vec3(0.0, i1.x, 1.0 ));

  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;

// Gradients: 41 points uniformly over a line, mapped onto a diamond.
// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;

// Normalise gradients implicitly by scaling m
// Approximation of: m *= inversesqrt( a0*a0 + h*h );
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );

// Compute final noise value at P
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}

float staticV(vec2 uv) {
    float staticHeight = snoise(vec2(9.0,runtime*1.2+3.0))*0.3+5.0;
    float staticAmount = snoise(vec2(1.0,runtime*1.2-6.0))*0.1+0.3;
    float staticStrength = snoise(vec2(-9.75,runtime*0.6-3.0))*2.0+2.0;
	return (1.0-step(snoise(vec2(5.0*pow(runtime,2.0)+pow(uv.x*7.0,1.2),pow((mod(runtime,100.0)+100.0)*uv.y*0.3+3.0,staticHeight))),staticAmount))*staticStrength;
}


void main() {
  if(red_splash) {
    color = texture(screenTexture, TexCoords);
    color.g = 0.2;
    color.b = 0.2;
  } else if(blue_splash) {
    color = texture(screenTexture, TexCoords);
    color.r = 0.2;
    color.g = 0.2;
  } else {
    color = texture(screenTexture, TexCoords);
  }

  if(TexCoords.y < lerp(0, 0.5, letterbox_percentage) || TexCoords.y > lerp(1, 0.5, letterbox_percentage))
    discard;

  float value = vignette(vec4(1, 1, 1, 1), 1.15, 0.25).r;
  if(dither(value, 0)) {
    color = color * vec4(1, 1, 1, 1);
  } else {
    discard;
  }


  // STATIC
  vertJerkOpt =     letterbox_percentage;
  vertMovementOpt = letterbox_percentage;
  bottomStaticOpt = letterbox_percentage;
  scalinesOpt =     letterbox_percentage;
  rgbOffsetOpt =    letterbox_percentage;
  horzFuzzOpt =     letterbox_percentage;
  if (paused == 1) {
    vertJerkOpt =     1.0;
    vertMovementOpt = 1.0;
    bottomStaticOpt = 1.0;
    scalinesOpt =     1.0;
    rgbOffsetOpt =    1.0;
    horzFuzzOpt =     1.0;
  }
  vertJerkOpt = 0.0;
  vertMovementOpt = 0.0;

  if(fast_foward == 1) {
    vertJerkOpt =     1.0;
    vertMovementOpt = 1.0;
    bottomStaticOpt = 1.0;
    scalinesOpt =     1.0;
    rgbOffsetOpt =    1.0;
    horzFuzzOpt =     1.0;
  }

  vec2 uv =  gl_FragCoord.xy/u_resolution.xy;

  float jerkOffset = (1.0-step(snoise(vec2(runtime*1.3,5.0)),0.8))*0.05;

  float fuzzOffset = snoise(vec2(runtime*15.0,uv.y*80.0))*0.003;
  float largeFuzzOffset = snoise(vec2(runtime*1.0,uv.y*25.0))*0.004;

    float vertMovementOn = (1.0-step(snoise(vec2(runtime*0.2,8.0)),0.4))*vertMovementOpt;
    float vertJerk = (1.0-step(snoise(vec2(runtime*1.5,5.0)),0.6))*vertJerkOpt;
    float vertJerk2 = (1.0-step(snoise(vec2(runtime*5.5,5.0)),0.2))*vertJerkOpt;
    float yOffset = abs(sin(runtime)*4.0)*vertMovementOn+vertJerk*vertJerk2*0.3;
    float y = mod(uv.y+yOffset,1.0);


  float xOffset = (fuzzOffset + largeFuzzOffset) * horzFuzzOpt;

    float staticVal = 0.0;

    for (float y = -1.0; y <= 1.0; y += 1.0) {
        float maxDist = 5.0/200.0;
        float dist = y/200.0;
      staticVal += staticV(vec2(uv.x,uv.y+dist))*(maxDist-abs(dist))*1.5;
    }

    staticVal *= bottomStaticOpt;

  float red 	=   texture(	screenTexture, 	vec2(uv.x + xOffset -0.01*rgbOffsetOpt,y)).r+staticVal;
  float green = 	texture(	screenTexture, 	vec2(uv.x + xOffset,	  y)).g+staticVal;
  float blue 	=	  texture(	screenTexture, 	vec2(uv.x + xOffset +0.01*rgbOffsetOpt,y)).b+staticVal;

  vec3 ccolor = vec3(red,green,blue);
  float scanline = sin(uv.y*800.0)*0.04*scalinesOpt;
  ccolor -= scanline;

  color = vec4(ccolor,1.0);

  // B&W
  if (paused == 1) {
    float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    color = vec4(average, average, average, 1.0);
  }
}
