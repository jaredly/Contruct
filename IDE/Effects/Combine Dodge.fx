// Combine Dodge
// Ashley Gullen
// PS 2.0
// Dodge blend the previous effect with the source image.

// Source texture
texture SourceTexture;

// Foreground texture
texture ForegroundTexture;

// Foreground sampler
sampler2D source = sampler_state {
    Texture = (SourceTexture);
    MinFilter = Point;
    MagFilter = Point;
    MipFilter = Point;
};

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
    float4 front = tex2D(source, Tex.xy);
    float4 back = tex2D(foreground, Tex.xy);
    front.rgb = (back.rgb * front.a) / (1.0f - front.rgb);
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
