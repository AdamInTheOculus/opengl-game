#version 330 core
out vec4 FragColor;

in vec3 CustomColor;
in vec2 TexCoord;

uniform sampler2D customTexture;
uniform sampler2D customTexture2;

void main()
{
    // Linearly interpolate between both textures (80% container, 20% awesomeface)
    FragColor = mix(texture(customTexture, TexCoord), texture(customTexture2, TexCoord), 0.2);
}