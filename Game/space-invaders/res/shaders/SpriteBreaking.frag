#version 330 core

layout(location = 0) out vec4 o_Color;

in vec2 v_UV;

uniform sampler2D u_Image;
uniform sampler2D u_BreakingImage;
uniform vec3 u_SpriteColor;
uniform float u_Integrity;

void main()
{          
    vec4 breakingColor = texture(u_BreakingImage, v_UV);
    float damageValue = 1.f - u_Integrity;
    breakingColor *= 1.f - sqrt(1.f - damageValue * damageValue);
    
    vec4 textureColor = texture(u_Image, v_UV);
    vec4 spriteColor = vec4(u_SpriteColor, 1.f);

    o_Color = mix(textureColor, breakingColor, breakingColor.a) * spriteColor;
}