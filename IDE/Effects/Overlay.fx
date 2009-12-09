// Overlay (Proper)
// Shviller
// PS 2.0
// A PROPER overlay blend.

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
    float4 front = tex2D(foreground, Tex.xy);
    float4 back = tex2D(background, Tex.xy);

    float4 output = front;
    output.r = back.r < 0.5 ? 2*back.r*front.r : 2*(front.r + back.r*front.a - back.r*front.r) - front.a;
    output.g = back.g < 0.5 ? 2*back.g*front.g : 2*(front.g + back.g*front.a - back.g*front.g) - front.a;
    output.b = back.b < 0.5 ? 2*back.b*front.b : 2*(front.b + back.b*front.a - back.b*front.b) - front.a;
    return output;
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
