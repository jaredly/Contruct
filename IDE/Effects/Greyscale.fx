// Greyscale
// Ashley Gullen
// PS 1.1
// Remove the colour from an image.

// Foreground texture
texture ForegroundTexture;

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
    float4 front = tex2D(foreground, Tex.xy);
    front.rgb = (front.r + front.g + front.b) / 3;
    return front;
}

// ConstructEffect
technique ConstructEffect
{
    pass p0
    {
        VertexShader = null;
        PixelShader = compile ps_1_1 EffectProcess();
    }
}
