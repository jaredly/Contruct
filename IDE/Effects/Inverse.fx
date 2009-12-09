// Inverse Plus
// Ashley Gullen
// PS 1.1
// Invert the colours, with intensity.

//#PARAM percent intensity 1 : Intensity : Intensity of the effect.
float intensity;

// Foreground texture
texture ForegroundTexture;

// Foreground sampler
sampler2D foreground = sampler_state {
    Texture = (ForegroundTexture);
    MinFilter = Point;
    MagFilter = Point;
    MipFilter = Point;
};

// Effect function
float4 EffectProcess( float2 Tex : TEXCOORD0 ) : COLOR0
{
    // Invert colours
    float4 front = tex2D(foreground, Tex.xy);
    front.rgb = lerp(front.rgb, 1 - front.rgb, intensity);
    front.rgb *= front.a;
    return front;
}

// ConstructEffect
technique ConstructEffect
{
    pass p0
    {
        VertexShader = null;
        PixelShader = compile ps_1_1 EffectProcess();
    }
}
