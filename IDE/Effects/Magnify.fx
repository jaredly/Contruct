// Magnify
// Ashley Gullen
// PS 1.4
// A glassy zoom on the background.

//#CROSS-SAMPLING : changes Tex.xy.
//#PARAM percent magnification 0.5 : Magnification factor : How much the background is magnified.
float magnification;

// Foreground texture
texture ForegroundTexture;

// Background texture
texture BackgroundTexture;

// Foreground sampler
sampler2D foreground = sampler_state {
    Texture = (ForegroundTexture);
    MinFilter = Point;
    MagFilter = Point;
    MipFilter = Point;
};

// OriginalTexture sampler
sampler2D background = sampler_state {
    Texture = (BackgroundTexture);
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
};

float hotspotX;
float hotspotY;

// Effect function
float4 EffectProcess( float2 Tex : TEXCOORD0 ) : COLOR0
{
    float4 p = tex2D(foreground, Tex.xy);
    float zoomFactor = (0.0f - (p.r + p.g + p.b) / 3) * magnification;

    Tex.x += (Tex.x - hotspotX) * zoomFactor;
    Tex.y += (Tex.y - hotspotY) * zoomFactor;

    return tex2D(background, Tex.xy);
}

// ConstructEffect
technique ConstructEffect
{
    pass p0
    {
        VertexShader = null;
        PixelShader = compile ps_1_4 EffectProcess();
    }
}
