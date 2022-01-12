#version 330 core
out vec4 FragColor;

in vec3 CustomColor;
in vec2 TexCoord;

uniform sampler2D customTexture;
uniform sampler2D customTexture2;
uniform float mixVariable;

void main()
{
    // FragColor = mix(texture(customTexture, TexCoord), texture(customTexture2, vec2(1.0 - TexCoord.x, TexCoord.y)), 0.2);

    // Linearly interpolate between both textures (80% container, 20% awesomeface)
    // FragColor = mix(
    //     texture(customTexture, TexCoord),
    //     texture(customTexture2, TexCoord),
    //     0.2
    // );

    // Same thing as above, but reverse the direction.
    FragColor = mix(
        texture(customTexture, TexCoord),
        texture(customTexture2, vec2(1.0 - TexCoord.x, TexCoord.y)),
        mixVariable
    );
}