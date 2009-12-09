// Blur Horizontal
// Ashley Gullen
// PS 2.0
// 13-sample Gaussian blur across 25 pixels.

//#BORDER-MODE : prevent pixel influence from outside box area.

// Foreground texture
texture ForegroundTexture;

// Foreground sampler
sampler2D foreground = sampler_state {
    Texture = (ForegroundTexture);
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
};

// Pixel width in texels
float pixelWidth;

float PixelKernel[13] =
{
    -11.5,
    -9.5,
    -7.5,
    -5.5,
    -3.5,
    -1.5,
     0.0,
     1.5,
     3.5,
     5.5,
     7.5,
     9.5,
     11.5,
};

static const float BlurWeights[13] = 
{
    0.002216,
    0.008764,
    0.026995,
    0.064759,
    0.120985,
    0.176033,
    0.199471,
    0.176033,
    0.120985,
    0.064759,
    0.026995,
    0.008764,
    0.002216,
};

// Effect function
float4 EffectProcess( float2 Tex : TEXCOORD0 ) : COLOR0
{
    // Apply surrounding pixels
    float4 color = 0;
    float2 samp = Tex;
    samp.y = Tex.y;

    for (int i = 0; i < 13; i++) {
        samp.x = Tex.x + PixelKernel[i] * pixelWidth;
        color += tex2D(foreground, samp.xy) * BlurWeights[i];
    }

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
