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
 
float2 Tex2 = Tex;
Tex2.x = (Tex2.x-0.5)* (1-Tex2.y*Tex2.y*Tex2.y*prog) + 0.5;
Tex2.y -=  Tex2.y*(prog*1 + 1*sin(Tex.x * 14 + prog*1) * sin(Tex.x * 30 + prog*12) * (40*pixelHeight*prog));


    float4 front = tex2D(foreground, Tex.xy);
    float4 back = tex2D(background, Tex2.xy);
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
