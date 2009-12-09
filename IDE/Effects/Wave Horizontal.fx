// Wave Horizontal
// David Clark
// PS 2.0
// Allows you to create cool water effects

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
float pixelHeight;

float frameCounter;

float RandomKernel[18] =
{
	0.13,
	0.38,
	0.41,
	-0.1,
	-0.46,
	0.03,
	0.05,
	0.27,
	0.4,
	0.12,
	0.13,
	0.38,
	0.41,
	-0.1,
	-0.46,
	0.03,
	0.05,
	0.27,	
};


// Effect function
float4 EffectProcess( float2 Tex : TEXCOORD0 ) : COLOR0
{
    // Apply surrounding pixels
    
    float2 Tex3 = Tex;
    
    int pos = (Tex.y / pixelWidth)% 18;
    
    Tex3.x += sin(RandomKernel[pos]*6.141 + frameCounter * 0.05)*8*pixelWidth;
    
   // RandomKernel[pos]* 8 * pixelWidth;
    

    return tex2D(foreground, Tex3.xy);
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
