// Offsets
// Ashley Gullen
// PS 2.0
// Allows you to offset an image

//#PARAM float offx 0 : Offset X : X offset.
float offx;

//#PARAM float offy 0 : Offset Y : Y offset.
float offy;

float pixelWidth;
float pixelHeight;

float boxWidth;
float boxHeight;

float boxLeft;
float boxRight;
float boxTop;
float boxBottom;

// Foreground texture
texture ForegroundTexture;

// Foreground sampler
sampler2D foreground = sampler_state {
    Texture = (ForegroundTexture);
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
};

// Effect function
float4 EffectProcess( float2 Tex : TEXCOORD0 ) : COLOR0
{
    // Add the front and back pixels
   Tex.x = (Tex.x + offx*pixelWidth - boxLeft ) % boxWidth + boxLeft; 
	Tex.y = (Tex.y + offy*pixelWidth - boxTop) % boxHeight + boxTop; 
   return tex2D(foreground, Tex.xy);
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
