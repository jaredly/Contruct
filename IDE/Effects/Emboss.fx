// Emboss
// Rich Whilding
// PS 2.0

// Foreground texture
texture ForegroundTexture;

// Foreground sampler
sampler2D foreground = sampler_state {
    Texture = (ForegroundTexture);
    MinFilter = Point;
    MagFilter = Point;
    MipFilter = Point;
};

float pixelWidth;
float pixelHeight;

// Effect function
float4 EffectProcess( float2 Tex : TEXCOORD0 ) : COLOR0
{
    // Find the average of the RGB and set all components to that value
    // creating a greyscale image
    float4 Color;
    Color.a = tex2D(foreground, Tex.xy).a;
    Color.rgb = 0.5f;
    Color -= tex2D(foreground, Tex.xy - float2(pixelWidth, pixelHeight))*2.0f;
    Color += tex2D(foreground, Tex.xy + float2(pixelWidth, pixelHeight))*2.0f;
    Color.rgb = (Color.r+Color.g+Color.b)/3.0f;
    Color.rgb *= Color.a;
    return Color;
}

// ConstructEffect
technique ConstructEffect
{
    pass p0
    {
        VertexShader = null;
        PixelShader = compile ps_2_0 EffectProcess();
    }
}
