#version 410 core

out vec4 FragColor;

in vec3 CustomColor;
in vec2 TexCoord;

uniform sampler2D customTexture;

void main()
{
    // FragColor = vec4(position, 1.0f);
    FragColor = texture(customTexture, TexCoord) * vec4(CustomColor, 1.0);
}