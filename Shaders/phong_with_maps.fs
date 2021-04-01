#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec4 directionality;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};


uniform Material material;
uniform Light light;

uniform vec3 viewPos;

void main()
{
    // Ambient
    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;

    // Diffuse
    vec3 norm = normalize(Normal);
    float w = light.directionality.w;
    vec3 rawLightDir = light.directionality.xyz*(2*w-1) - FragPos*w;
    vec3 lightDir = normalize(rawLightDir);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(texture(material.diffuse, TexCoords)) * light.diffuse;

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular =  spec * vec3(texture(material.specular, TexCoords)) * light.specular;

    // Attenuation
    float distance    = length(rawLightDir);
    float attenuation = pow(1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)), w); 

    FragColor = vec4(ambient*attenuation + diffuse*attenuation + specular*attenuation, 1.0);
}