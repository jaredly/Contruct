// Separate RGB
// David Clark
// PS 2.0
// Allows you to offset each colour channel

//#BORDER-MODE : prevent pixel influence from outside box area.
//#PARAM float redx 0 : Red X offset : Offset in pixels.
//#PARAM float greenx 0 : Green X offset : Offset in pixels.
//#PARAM float bluex 0 : Blue X offset : Offset in pixels.
//#PARAM float redy 0 : Red Y offset : Offset in pixels.
//#PARAM float greeny 0 : Green Y offset : Offset in pixels.
//#PARAM float bluey 0 : Blue Y offset : Offset in pixels.

// Parameter variables
float redx;
float redy;
float bluex;
float bluey;
float greenx;
float greeny;


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

// Effect function
float4 EffectProcess( float2 Tex : TEXCOORD0 ) : COLOR0
{
    // Apply surrounding pixels
    
    float2 Tex2 = Tex;
    float4 col = tex2D(foreground, Tex2);
    col.r = tex2D(foreground,Tex2 + float2(redx * pixelWidth, redy * pixelHeight)).r;
    col.g = tex2D(foreground,Tex2 + float2(greenx * pixelWidth, greeny * pixelHeight)).g;
    col.b = tex2D(foreground,Tex2 + float2(bluex * pixelWidth, bluey * pixelHeight)).b;  

    return col;
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
