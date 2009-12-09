// Soften
// Ashley Gullen
// PS 2.0
// 5-sample 17 pixel average.

//#BORDER-MODE : prevent pixel influence from outside box area.

// Foreground texture
texture ForegroundTexture;

// Foreground sampler
sampler2D foreground = sampler_state {
    Texture = (ForegroundTexture);
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
};

float pixelWidth;
float pixelHeight;

// Effect function
float4 EffectProcess( float2 Tex : TEXCOORD0 ) : COLOR0
{
	float halfPixelWidth = pixelWidth / 2;
	float halfPixelHeight = pixelHeight / 2;
	float deltaWidth = pixelWidth + halfPixelWidth;
	float deltaHeight = pixelHeight + halfPixelHeight;

	float4 color = 0;
	color += tex2D(foreground, Tex.xy + float2(deltaWidth, halfPixelHeight)) * 0.2f;
	color += tex2D(foreground, Tex.xy + float2(-halfPixelWidth, deltaHeight)) * 0.2f;
	color += tex2D(foreground, Tex.xy + float2(-deltaWidth, -halfPixelHeight)) * 0.2f;
	color += tex2D(foreground, Tex.xy + float2(halfPixelWidth, -deltaHeight)) * 0.2f;
	color += tex2D(foreground, Tex.xy) * 0.2f;

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
