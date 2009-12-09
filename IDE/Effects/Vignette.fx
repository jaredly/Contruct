// Vignette
// Jeff Attwood
// PS 2.0
// Corners of the screen are darkened.

//#PARAM float enabled 1 : Active: Is this effect Active? (0 = Off, 1 = On).
float enabled;
//#PARAM float intensity 3 : Intensity : Intensity of the effect. ( 0 - 5 are acceptable values)
float intensity;
//#PARAM percent darkness 1.5 : Darkness : Effect Darkness (150% = Normal)
float darkness;

// Pretty sure these are working...
float boxWidth;
float boxHeight;

// Foreground texture
texture ForegroundTexture;

// Background texture
texture BackgroundTexture;

sampler2D foreground = sampler_state {
// Foreground sampler
    Texture = (ForegroundTexture);
    MinFilter =	Point;
    MagFilter = Point;
	AddressU = Wrap;
    AddressV = Wrap;
};
sampler2D background = sampler_state {
    Texture = (BackgroundTexture);
    MinFilter = Point;
    MagFilter = Point;
    MipFilter = Point;
};
 
// Pixel Shader Output
float4 EffectProcess( float2 Tex : TEXCOORD0  ) : COLOR0 
{
	if (enabled == 1)
	{
		float4 back = tex2D(background, Tex.xy);
		
		Tex.x -= boxWidth * 0.5;
		Tex.y -= boxHeight * 0.5;
		
		//lets start.
		float3 cResult = back.rgb;
		float vignette  = 1 - dot( Tex, Tex );
		cResult *= pow( vignette, intensity );
		cResult = pow( cResult, 0.680 * darkness );
		
		// return with source alpha
		return float4(cResult, back.a);
	}
	else
	{
	return tex2D( foreground, Tex.xy);
	}
} 

//Technique Calls
technique ConstructEffect 
{ 
	pass P0
	{ 
		VertexShader = null;
		PixelShader = compile ps_2_0 EffectProcess(); 	  
	} 
} 