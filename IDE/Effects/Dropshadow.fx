// Dropshadow
// David Clark
// PS 2.0

//#CROSS-SAMPLING : reads pixels it may be writing.
//#BORDER-MODE : samples pixels outside bounding box

// Foreground texture
texture ForegroundTexture;

// Foreground sampler
sampler2D foreground = sampler_state {
    Texture = (ForegroundTexture);
    MinFilter = Point;
    MagFilter = Point;
    MipFilter = Point;
};

float pixelWidth;
float pixelHeight;

// Effect function
float4 EffectProcess( float2 Tex : TEXCOORD0 ) : COLOR0
{
    // Add the front and back pixels
    float2 Tex2 = Tex;
    Tex2.x -= 4 * pixelWidth;
    Tex2.y -= 4 * pixelHeight;
    float4 here = tex2D(foreground, Tex2.xy);
    float4 left = tex2D(foreground, float2(Tex2.x - pixelWidth, Tex2.y));
    float4 right = tex2D(foreground, float2(Tex2.x + pixelWidth, Tex2.y));
    float4 top = tex2D(foreground, float2(Tex2.x, Tex2.y - pixelHeight));
    float4 bottom = tex2D(foreground, float2(Tex2.x, Tex2.y + pixelHeight));

    float4 result = (here + left + right + top + bottom )/ 5;
    float4 src = tex2D(foreground, Tex.xy);
    
    result.rgb = 0;
    
    return result * (1-src.a) + src;

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
