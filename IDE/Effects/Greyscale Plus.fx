// Greyscale Plus
// Ashley Gullen
// PS 2.0
// Greyscale with intensity.

// Convert an image to shades of grey by making the r, g and b components
// the same.
//#PARAM percent intensity 1 : Intensity : Intensity of the effect.
float intensity;

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
    float3 grey = (front.r + front.g + front.b) / 3;
    front.rgb = lerp(front.rgb, grey, intensity);
    return front;
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
