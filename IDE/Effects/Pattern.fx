// Pattern
// Ashley Gullen
// PS 2.0
// Allows you to apply scales, perspective, and offset effects

//#PARAM float offx 0 : Offset X : X offset.
float offx;

//#PARAM float offy 0 : Offset Y : Y offset.
float offy;

//#PARAM percentage topscale 0.1 : Top Scale : Top Scale.
float topscale;

//#PARAM percentage bottomscale 1 : Bottom Scale : Bottom Scale.
float bottomscale;

//#PARAM percentage leftscale 1 : Left Scale : Left Scale.
float leftscale;

//#PARAM percentage rightscale 1 : Right Scale : Right Scale.
float rightscale;



float pixelWidth;
float pixelHeight;

float boxWidth;
float boxHeight;

float boxLeft;
float boxRight;
float boxTop;
float boxBottom;

// Foreground texture
texture ForegroundTexture;

// Foreground sampler
sampler2D foreground = sampler_state {
    Texture = (ForegroundTexture);
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
};

// Effect function
float4 EffectProcess( float2 Tex : TEXCOORD0 ) : COLOR0
{
    // Add the front and back pixels
	Tex.x -= boxLeft;
	Tex.y -= boxTop;
	
	float2 Scale = Tex;
	Scale.x /= boxWidth;
	Scale.y /= boxHeight;

	float scalex = lerp(leftscale, rightscale, Scale.x);
	float scaley = lerp(topscale, bottomscale, Scale.y);
	
	Tex.x -= boxWidth * 0.5;
	Tex.y -= boxHeight * 0.5;

	Tex.x /= scaley;
	Tex.y /= scalex;

	Tex.x += offx * pixelWidth;
	Tex.y += offy * pixelHeight;
	
	//return float4(Scale.x,Scale.x, Scale.x,Scale.x);
	Tex.x += boxWidth*1000.5;
	Tex.y += boxHeight*1000.5;
	Tex.x = (Tex.x % boxWidth) + boxLeft; 
	Tex.y = (Tex.y % boxHeight) + boxTop; 
	return tex2D(foreground, Tex.xy);
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
