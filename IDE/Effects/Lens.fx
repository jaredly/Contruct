// Lens
// Ashley Gullen
// PS 1.4
// Zoom or magnify the background.

//#CROSS-SAMPLING : changes Tex.xy.
//#PARAM percent magnification 1 : Magnification factor : How much the background is magnified.
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
	if( p.a != 0 )
	{
		 float zoomFactor = (0.5f - p.r) * magnification;

		 Tex.x += (Tex.x - hotspotX) * zoomFactor;
		 Tex.y += (Tex.y - hotspotY) * zoomFactor;

		 p = tex2D(background, Tex.xy);
	 }
    return p;
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
