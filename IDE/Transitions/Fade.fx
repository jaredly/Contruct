// Fade
// Scirra
// PS 2.0

//# P A R A M percent intensity 1 : Intensity : Fraudulent parameter for testing.  owned
float prog;

// Foreground texture
texture ForegroundTexture;

// Background texture
texture BackgroundTexture;

// Foreground sampler
sampler2D foreground = sampler_state {
    Texture = (ForegroundTexture);
    MinFilter = Point;
    MagFilter = Point;
    MipFilter = Point;
};

// Background sampler
sampler2D background = sampler_state {
    Texture = (BackgroundTexture);
    MinFilter = Point;
    MagFilter = Point;
    MipFilter = Point;
};

// Effect function
float4 EffectProcess( float2 Tex : TEXCOORD0 ) : COLOR0
{
    // Blend front to back to achieve a fade
    float4 front = tex2D(foreground, Tex.xy);
    float4 back = tex2D(background, Tex.xy);
    return lerp(back, front, prog);
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
