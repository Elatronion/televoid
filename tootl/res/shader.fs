#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Light light;

layout(origin_upper_left) in vec4 gl_FragCoord;

uniform sampler2D ourTexture;

uniform int transparent;

void main() {
  if(texture(ourTexture, TexCoord).a == 0) discard;
  // ambient
  vec3 ambient = light.ambient * texture(ourTexture, TexCoord).rgb;

  // diffuse
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(light.position - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * texture(ourTexture, TexCoord).rgb;

  // specular
  /*vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;*/

  // spotlight (soft edges)
  /*float theta = dot(lightDir, normalize(-light.direction));
  float epsilon = (light.cutOff - light.outerCutOff);
  float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
  diffuse  *= intensity;
  specular *= intensity;*/

  // attenuation
  /*float distance    = length(light.position - FragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
  ambient  *= attenuation;
  diffuse   *= attenuation;
  //specular *= attenuation;*/

  vec3 result = (ambient + diffuse/* + specular*/);

  FragColor = vec4(result, 1.0);
  FragColor = texture(ourTexture, TexCoord);
}
