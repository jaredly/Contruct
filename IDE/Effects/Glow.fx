// Glow
// David Clark
// PS 2.0
// Use after a blur for a glow effect.

//#PARAM percent exposure 1 : Overdrive : Overdrive the glow effect.
float exposure;

// Foreground texture
texture ForegroundTexture;

// Source texture
texture SourceTexture;

// Foreground sampler
sampler2D foreground = sampler_state {
    Texture = (ForegroundTexture);
    MinFilter = Point;
    MagFilter = Point;
    MipFilter = Point;
};

// Source sampler
sampler2D source = sampler_state {
    Texture = (SourceTexture);
    MinFilter = Point;
    MagFilter = Point;
    MipFilter = Point;
};

// Effect function
float4 EffectProcess( float2 Tex : TEXCOORD0 ) : COLOR0
{
    // Add the front and back pixels
    float4 original = tex2D(source, Tex.xy);
    float4 blur = tex2D(foreground, Tex.xy)*exposure;
    float4 color = max(original, blur);

    return color;
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
