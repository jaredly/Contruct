// Ambient Shadow
// David Clark
// PS 2.0
// Description here

// Foreground texture
texture ForegroundTexture;

//#PARAM float spread 2.5 : Spread : The range of the blur
float spread;

//#PARAM float cutoff 0.1 : Cutoff : Limits how far objects can cast a shadow
float cutoff;

//#PARAM float threshold 0.01 : Threshold : Must cross this threshold to cast a shdow
float threshold;

//#PARAM float intensity 1.5 : Intensity : Intensity of shadow.
float intensity;

// Foreground sampler
sampler2D foreground = sampler_state {
    Texture = (ForegroundTexture);
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
};

// Pixel width in texels
float pixelWidth;

// Pixel height in texels
float pixelHeight;

static const int numSamples = 16;

static const float2 samples[numSamples] =
{
	float2(0.007937789, 0.73124397),
	float2(-0.10177308, -0.6509396),
	float2(-0.9906806, -0.63400936),
	float2(-0.5583586, -0.3614012),
	float2(0.7163085, 0.22836149),
	float2(-0.65210974, 0.37117887),
	float2(-0.12714535, 0.112056136),
	float2(0.48898065, -0.66669613),
	float2(-0.9744036, 0.9155904),
	float2(0.9274436, -0.9896486),
	float2(0.9782181, 0.90990245),
	float2(0.96427417, -0.25506377),
	float2(-0.5021933, -0.9712455),
	float2(0.3091557, -0.17652994),
	float2(0.4665941, 0.96454906),
	float2(-0.461774, 0.9360856)
};

static const float2 depthOffset = float2(0.001, 0.003);



// Effect function
float4 EffectProcess( float2 Tex : TEXCOORD0 ) : COLOR0
{
	float2 baseCoord = Tex;
	float baseDepth = tex2D(foreground, baseCoord).r;
		
	float ambient = 1.0;	
	for (int i = 0; i < numSamples; i++)
	{
		float2 offsetPixel = (depthOffset + (samples[i])) * float2(pixelWidth, pixelHeight) * spread;
		float offsetDepth = tex2D(foreground, baseCoord + offsetPixel).r;
		float diff = offsetDepth - baseDepth; // diff is +itive if offset depth
														// is in front of us
 
		if (diff < cutoff && diff > threshold)
		{
			diff = clamp(diff, 0.0, cutoff);
			diff = cutoff - diff;
 
			ambient -= diff;
		}
	}
   return float4(0, 0, 0, (1.0 - ambient) * intensity);
}

// ConstructEffect
technique ConstructEffect
{
    pass p0
    {
        VertexShader = null;
        PixelShader = compile ps_3_0 EffectProcess();
    }
}
