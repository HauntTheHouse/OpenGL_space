#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube space;

void main()
{
    vec4 color = texture(space, TexCoords);
    float bright = 0.6f;
    if(color.x < bright && color.y < bright && color.z < bright)
        color *= 0.2f;
    FragColor = color;
}