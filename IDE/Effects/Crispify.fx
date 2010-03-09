// Crispify
// Ashley Gullen
// PS 1.1
// Harden edges of linear-filtered objects resized big.

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

// Foreground sampler
sampler2D background = sampler_state {
    Texture = (BackgroundTexture);
    MinFilter = Point;
    MagFilter = Point;
    MipFilter = Point;
};

// Effect function
float4 EffectProcess( float2 Tex : TEXCOORD0 ) : COLOR0
{
    // Crispen up
    float4 front = tex2D(foreground, Tex.xy);

	front.a -= 0.45;
	front.a *= 10.0;
	front.a = clamp(front.a, 0, 1);
   
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
