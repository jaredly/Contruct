// Tint Plus
// Ashley Gullen
// PS 2.0
// Tint or boost the red, green and blue channels.

//#PARAM percent aRed 1 : Inner Red : Inside Red
//#PARAM percent aGreen 1 : Inner Green : Inner Green
//#PARAM percent aBlue 0.4 : Inner Blue : Inner Blue 
//#PARAM percent aOpacity 1 : Inner Opacity : Inner Opacity

//#PARAM percent bRed 0.8 : Outter Red : Outter Red
//#PARAM percent bGreen 0 : Outter Green : Outter Green
//#PARAM percent bBlue 0 : Outter Blue : Outter Blue 
//#PARAM percent bOpacity 1 : Outter Opacity : Outter Opacity

//#PARAM percent Clip 0.1: Clip Percentage:Determines where the edges end

// Parameter variables
float aRed;
float aGreen;
float aBlue;
float aOpacity;
float bRed;
float bGreen;
float bBlue;
float bOpacity;
float Clip;

// Foreground texture
texture ForegroundTexture;

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
	float4 flame = tex2D(foreground, Tex.xy);
	
	
	if(flame.a < Clip)
		flame = float4(0,0,0,0);
	else
	flame = lerp(float4(bRed*bOpacity, bGreen*bOpacity, bBlue*bOpacity, bOpacity),
			    float4(aRed*aOpacity, aGreen*aOpacity, aBlue*aOpacity, aOpacity),
	            flame.a);
	return flame;
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
