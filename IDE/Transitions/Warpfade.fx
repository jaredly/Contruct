// Warpfade
// Scirra
// PS 2.0

float prog;
float pixelHeight;

// Foreground texture
texture ForegroundTexture;

// Background texture
texture BackgroundTexture;

// Foreground sampler
sampler2D foreground = sampler_state {
    Texture = (ForegroundTexture);
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
};

// Background sampler
sampler2D background = sampler_state {
    Texture = (BackgroundTexture);
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
};

// Effect function
float4 EffectProcess( float2 Tex : TEXCOORD0 ) : COLOR0
{
    // Blend front to back to achieve a fade
    if (prog <= 0.5)
  	  Tex.y += sin(Tex.y * 25 + prog * 5) * (20 * pixelHeight * prog);
    else
	  Tex.y += sin(Tex.y * 25 + prog * 5) * (20 * pixelHeight * (1 - prog));

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
