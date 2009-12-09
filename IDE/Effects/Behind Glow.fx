// Behind Glow
// David Clark
// PS 2.0
// Use after a blur for a behind-glow effect.

//#PARAM percent exposure 1 : Overdrive : Overdrive the glow effect.
float exposure;

//#PARAM percent fRed 1 : Red : Red tint.
//#PARAM percent fGreen 1 : Green : Green tint.
//#PARAM percent fBlue 1 : Blue : Blue tint.


// Parameter variables
float fRed;
float fGreen;
float fBlue;

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
    float4 bg = tex2D(foreground, Tex.xy)*exposure;
    bg.r = fRed;
    bg.g = fGreen;
    bg.b = fBlue;
    bg.rgb *= bg.a;

    return bg * (1-original.a) + original;
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
