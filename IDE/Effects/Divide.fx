// Divide
// Ashley Gullen
// PS 2.0
// Division blend with intensity.

//#PARAM percent intensity 1 : Intensity : Intensity of the effect.
float intensity;

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

// OriginalTexture sampler
sampler2D background = sampler_state {
    Texture = (BackgroundTexture);
    MinFilter = Point;
    MagFilter = Point;
    MipFilter = Point;
};

// Effect function
float4 EffectProcess( float2 Tex : TEXCOORD0 ) : COLOR0
{
    // Divide the background by the foreground
    float4 front = tex2D(foreground, Tex.xy);
    float4 back = tex2D(background, Tex.xy);

    // Calculate the divisor based on foreground color and intensity
    float3 divisor = (1 - ((1 - front.rgb) * intensity));

    // Above 100% intensity, it may cap and go negative, causing black areas
    // Cap at white instead
    divisor = max(divisor, float3(0,0,0));

    // Perform division
    front.rgb = back.rgb / divisor;
    return front;
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
