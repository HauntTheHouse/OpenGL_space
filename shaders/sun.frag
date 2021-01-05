#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform vec3 lightColor;

uniform sampler2D sunTexture;

void main()
{
    FragColor = texture(sunTexture, TexCoord) * vec4(lightColor, 1.0f);
}