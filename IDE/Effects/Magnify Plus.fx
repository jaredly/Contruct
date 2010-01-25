// Magnify
// David Clark
// PS 2.0
// A glassy zoom on the background.

//#CROSS-SAMPLING : changes Tex.xy.
//#PARAM percent magnification 0.5 : Magnification factor : How much the background is magnified.
float magnification;

//#PARAM float offx 0 : Offset X : X offset.
float offx;

//#PARAM float offy 0 : Offset Y : Y offset.
float offy;

float pixelWidth;
float pixelHeight;

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
	 if( p.a != 0)
	 {
		float zoomFactor = (0.0f - (p.r + p.g + p.b) / 3) * magnification;

		Tex.x += (Tex.x - hotspotX + offx * pixelWidth) * zoomFactor;
		Tex.y += (Tex.y - hotspotY + offy * pixelHeight) * zoomFactor;
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
        PixelShader = compile ps_2_0 EffectProcess();
    }
}
