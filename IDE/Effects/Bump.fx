// Bump
// David
// PS 2.0

//#BORDER-MODE : prevent pixel influence from outside box area.
// Foreground texture
texture ForegroundTexture;

//#PARAM percent intensity 1 : Intensity : Intensity of the effect.
float intensity;

float pixelWidth;
float pixelHeight;

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
    // Find the average of the RGB and set all components to that value
    // creating a greyscale image
    float4 Color;
    Color.a = 1.0f;
    Color.rgb = 0.5f;
   float diff =  tex2D(foreground, float2(Tex.x+pixelWidth,Tex.y+pixelHeight)).r - tex2D(foreground, float2(Tex.x-pixelWidth,Tex.y-pixelHeight)).r;
    diff *= intensity;
   if(diff > 0)
{
          Color.rgb = diff;
          Color.a = 0;
}
else
 {
       Color.rgb = 0;
       Color.a = -diff;
}

    return Color;
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
