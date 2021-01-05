#version 330 core
out vec4 FragColor;

uniform vec3 lightColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D earthTexture;
uniform sampler2D nightEarthTexture;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

void main()
{
    vec3 norm = normalize(Normal);

    float ambientStrength = 0.03f;
    vec3 ambient = ambientStrength * lightColor;

    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.8f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir),0.0), 2);
    vec3 specular = specularStrength * spec * lightColor;

    vec4 result = vec4((ambient + diffuse + specular), 1.0f) * texture(earthTexture, TexCoord);
    if(dot(norm, lightDir) > 0.0f && dot(norm, lightDir) <= 1.0f)
        result += texture(nightEarthTexture, TexCoord) * pow(0.0001, dot(norm, lightDir)) * 1.1f;
    if(dot(norm, lightDir) <= 0.0f)
        result += texture(nightEarthTexture, TexCoord) * 1.1f;
    FragColor = result;
}