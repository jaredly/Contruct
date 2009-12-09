// Flip Horizontal
// Ashley Gullen
// PS 1.4
// Makes a mirror image.

float boxLeft;
float boxRight;

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
    // Add the front and back pixels
    Tex.x = boxRight - (Tex.x - boxLeft);
    return tex2D(foreground, Tex.xy);
}

// ConstructEffect
technique ConstructEffect
{
    pass p0
    {
        VertexShader = null;
        PixelShader = compile ps_1_4 EffectProcess();  
    }
}
