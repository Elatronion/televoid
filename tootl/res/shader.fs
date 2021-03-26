#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct Material {
  sampler2D diffuse;
  sampler2D normal;
};

#define NR_POINT_LIGHTS 10

uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform Material material;

layout(origin_upper_left) in vec4 gl_FragCoord;

uniform int transparent;

vec3 CalcDirLight(DirLight light, vec3 normal)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    //vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse/* + specular*/);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos)
{
  // ambient
  vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;

  // diffuse
  vec3 norm = normalize(normal);
  vec3 lightPos = light.position.xyz;//vec3(light.position.xy, FragPos.z + 15);
  vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;

  // attenuation
  float distance    = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance) + 0.0001);
  ambient *= attenuation;
  diffuse *= attenuation;

  return (ambient + diffuse);
}

void main() {
  if(texture(material.diffuse, TexCoord).a == 0) discard;

  bool do_lighting = false;
  if(do_lighting) {
    vec3 normal = texture(material.normal, TexCoord).rgb;
    vec3 result = CalcDirLight(dirLight, normal);

    for(int i = 0; i < NR_POINT_LIGHTS; i++)
          result += CalcPointLight(pointLights[i], normal, FragPos);

    FragColor = vec4(result, 1.0);
  } else {
    FragColor = texture(material.diffuse, TexCoord);
    if(FragColor.a == 0) discard;
  }
}
