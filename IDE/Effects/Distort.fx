//  Distort
// David Clark
// PS 2
// Distortion

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

float pixelWidth;
float pixelHeight;

// Effect function
float4 EffectProcess( float2 Tex : TEXCOORD0 ) : COLOR0
{
	float4 col = tex2D(foreground, Tex.xy);
	
	if( col.a != 0 ) 
	{
		float src = col.r;
		float diffx = src - tex2D(foreground, Tex.xy +  float2(pixelWidth, 0)).r;
		float diffy = src - tex2D(foreground, Tex.xy +  float2(0,pixelHeight)).r;

		Tex.x += diffx * magnification * pixelWidth *64 *col.a;
		Tex.y += diffy * magnification * pixelHeight * 64 * col.a;
		col = tex2D(background, Tex.xy);
	 }

    return col;
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
