#version 330 core

layout(location = 0) out vec4 o_Color;

uniform sampler2D u_ScreenTexture;
uniform sampler2D u_VelocityTexture;
uniform bool u_GrayscaleEnabled;

in vec2 v_UV;

vec4 CalcMotionBlur(vec2 velocity);

void main()
{          
    vec2 velocity = texture(u_VelocityTexture, v_UV).rg / 2.f;

    vec4 colorResult = CalcMotionBlur(velocity);
    
    if(u_GrayscaleEnabled)
    {
        float average = 0.2126f * colorResult.r + 0.7152f * colorResult.g + 0.0722f * colorResult.b;
        colorResult = vec4(average, average, average, 1.f);
    }

    o_Color = colorResult;
}

vec4 CalcMotionBlur(vec2 velocity)
{    
    // Ogldev's approach (https://ogldev.org/www/tutorial41/tutorial41.html)
    vec2 uv = v_UV;
    vec4 colorResult = vec4(0.f);
    colorResult += texture(u_ScreenTexture, uv) * 0.7f;
    uv += velocity;
    colorResult += texture(u_ScreenTexture, uv) * 0.2f;
    uv += velocity;
    colorResult += texture(u_ScreenTexture, uv) * 0.1f;
    
    // Nvidia's approach (https://developer.nvidia.com/gpugems/gpugems3/part-iv-image-effects/chapter-27-motion-blur-post-processing-effect)        
    // const int motionBlurNumSamples = 2;
    // vec4 colorResult = texture(u_ScreenTexture, v_UV);
    // vec4 color = colorResult;
    // vec2 uv = v_UV;
    // uv += velocity;

    // for(int i = 1; i < motionBlurNumSamples; ++i, uv += velocity) 
    // {   
    //     color += texture(u_ScreenTexture, uv);
    // }
    
    // colorResult = color / motionBlurNumSamples;

    return colorResult;
}