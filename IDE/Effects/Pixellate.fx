// Pixellate
// Ashley Gullen
// PS 2.0
// Lower the resolution of an image.

//#PARAM float xPixellation 50 : X Resolution : The horizontal sharpness of the image.
//#PARAM float yPixellation 50 : Y Resolution : The vertical sharpness of the image.
float xPixellation;
float yPixellation;
float pixelWidth;
float pixelHeight;

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
    // Add the front and back pixels
    Tex.x = floor(Tex.x * xPixellation) / xPixellation + pixelWidth;
    Tex.y = floor(Tex.y * yPixellation) / yPixellation + pixelHeight;
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
