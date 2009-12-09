// HDR tonemapping
// Ashley Gullen
// PS 2.0
// Use after a blur for a lighting effect.

//#PARAM float exposure 2.5 : Exposure : Camera exposure of the HDR effect.
float exposure;

// Foreground texture
texture ForegroundTexture;

// Source texture
texture SourceTexture;

// Foreground sampler
sampler2D foreground = sampler_state {
    Texture = (ForegroundTexture);
    MinFilter = Point;
    MagFilter = Point;
    MipFilter = Point;
};

// Source sampler
sampler2D source = sampler_state {
    Texture = (SourceTexture);
    MinFilter = Point;
    MagFilter = Point;
    MipFilter = Point;
};

// Effect function
float4 EffectProcess( float2 Tex : TEXCOORD0 ) : COLOR0
{
    // Add the front and back pixels
    float4 original = tex2D(source, Tex.xy);
    float4 blur = tex2D(foreground, Tex.xy);
    float4 color = lerp(original, blur, 0.4f);

    // Calculate vignette
    Tex -= 0.5;
    float v = 1 - dot(Tex, Tex);

    // Apply vignette to color
    color.rgb *= pow(v, 4.0);

    // Exposure level
    color.rgb *= exposure;

    color.rgb = pow(color.rgb, 0.55);
    
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
