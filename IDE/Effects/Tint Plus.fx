// Tint Plus
// Ashley Gullen
// PS 2.0
// Tint or boost the red, green and blue channels.

//#PARAM percent fRed 1 : Red : Red tint.
//#PARAM percent fGreen 0 : Green : Green tint.
//#PARAM percent fBlue 0 : Blue : Blue tint.


// Parameter variables
float fRed;
float fGreen;
float fBlue;

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
    return tex2D(foreground, Tex.xy) * float4(fRed, fGreen, fBlue, 1);
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
