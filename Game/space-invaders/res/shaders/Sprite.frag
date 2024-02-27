#version 330 core

layout(location = 0) out vec4 o_Color;

in vec2 v_UV;

uniform sampler2D u_Image;
uniform vec3 u_SpriteColor;

void main()
{          
    //o_Color = vec4(u_SpriteColor, 1.f) * texture(u_Image, v_UV);
    o_Color = vec4(1.f);
}