// Contrast
// Jeff Attwood
// PS 1.4
// Contrast Shader

//#BORDER-MODE : prevent pixel influence from outside box area.

//#PARAM percent brightness 1 : Brightness : Overall Brightness.
float brightness;

//#PARAM percent saturation 1 : Saturation : Final Saturation of the image. 0% would be black and white.
float saturation;

//#PARAM percent contrast 1 : Constrast : Final Constrast of the image.
float contrast;

// Foreground texture
texture ForegroundTexture;

sampler2D foreground = sampler_state {
// Foreground sampler
    Texture = (ForegroundTexture);
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
};

//Contast, Saturation, Brightness 
float3 CSB(float3 color, float brightness, float saturation, float contrast)
{	
	// Increase or decrease theese values to adjust r, g and b color channels seperately
	const float AvgLumR = 0.5;
	const float AvgLumG = 0.5;
	const float AvgLumB = 0.5;
	
	const float3 LumCoeff = float3(0.2125, 0.7160, 0.0722);
	
	float3 AvgLumin = float3(AvgLumR, AvgLumG, AvgLumB);
	float3 brightnessColor = color * brightness;
	float intensityf = dot(brightnessColor, LumCoeff);
	float3 intensity = float3(intensityf, intensityf, intensityf);
	float3 satColor = lerp(intensity, brightnessColor, saturation);
	float3 conColor = lerp(AvgLumin, satColor, contrast);
	return conColor;
}

// Effect function
float4 EffectProcess( float2 Tex : TEXCOORD0 ) : COLOR0
{
	float4 color = tex2D(foreground, Tex.xy);
	
	color.rgb = CSB(color.rgb, brightness, saturation, contrast);
	return color.rgba;
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
