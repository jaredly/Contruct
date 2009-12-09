// BlurMask Horizontal
// Ashley Gullen
// PS 2.0
// Gaussian blur on the background using a foreground mask.

//#CROSS-SAMPLING

// Source texture
texture SourceTexture;

// Previous texture
texture PreviousTexture;

// Foreground sampler
sampler2D source = sampler_state {
    Texture = (SourceTexture);
    MinFilter = Point;
    MagFilter = Point;
    MipFilter = Point;
};

// Background sampler (using PreviousTexture since a mask)
sampler2D background = sampler_state {
    Texture = (PreviousTexture);
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
     0.5,
     2.5,
     4.5,
     6.5,
     8.5,
     10.5,
     12.5,
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
    float weight = tex2D(source, Tex.xy).r;

    for (int i = 0; i < 13; i++) {
        samp.x = Tex.x + PixelKernel[i] * pixelWidth * weight;
        color += tex2D(background, samp.xy) * BlurWeights[i];
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
