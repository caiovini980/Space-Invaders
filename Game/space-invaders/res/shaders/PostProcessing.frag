#version 330 core

layout(location = 0) out vec4 o_Color;

uniform sampler2D u_ScreenTexture;
uniform bool u_GrayscaleEnabled;

in vec2 v_UV;

void main()
{          
    vec4 colorResult = texture(u_ScreenTexture, v_UV);

    if(u_GrayscaleEnabled)
    {
        float average = 0.2126f * colorResult.r + 0.7152f * colorResult.g + 0.0722f * colorResult.b;
        colorResult = vec4(average, average, average, 1.f);
    }

    o_Color = colorResult;
}