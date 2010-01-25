// Multiply
// Ashley Gullen
// PS 2.0
// Multiplication blend with intensity.


//#PARAM percent intensity 1 : Intensity : Intensity of the effect.
float intensity;

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
    MinFilter = Point;
    MagFilter = Point;
    MipFilter = Point;
};

// Effect function
float4 EffectProcess( float2 Tex : TEXCOORD0 ) : COLOR0
{
    float4 front = tex2D(foreground, Tex.xy);
	 if(front.a != 0)
	 {
		float4 back = tex2D(background, Tex.xy);
		front.a *= back.a;
		front.rgb *= back.rgb * intensity;
	 }
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
