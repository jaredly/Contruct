// Warp Bounded
// David Clark
// PS 2.0
// Animated wave distortion, but the wave wont leave the boundaries of the alpha channel.

//#BORDER-MODE : prevents clipping to the bounding box area.
//#PARAM float xFreq 25 : X Frequency : Frequency multiplier of the horizontal waves.
//#PARAM float yFreq 25 : Y Frequency : Frequency multiplier of the vertical waves.
//#PARAM float xAmplitude 5 : X Amplitude : Size of the horizontal waves, in pixels.
//#PARAM float yAmplitude 5 : Y Amplitude : Size of the vertical waves, in pixels.
//#PARAM percent xSpeed 0.025 : X Speed : Speed multiplier for horizontal waves.
//#PARAM percent ySpeed 0.025 : Y Speed : Speed multiplier for vertical waves.

// Parameter variables
float xFreq;
float yFreq;
float xAmplitude;
float yAmplitude;
float xSpeed;
float ySpeed;

// Foreground texture
texture ForegroundTexture;

// Foreground sampler
sampler2D foreground = sampler_state {
    Texture = (ForegroundTexture);
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
};

// Increments 1 per frame
float frameCounter;

float boxLeft;
float boxTop;
float pixelWidth;
float pixelHeight;

// Effect function
float4 EffectProcess( float2 Tex : TEXCOORD0 ) : COLOR0
{
    // Change the pixel to sample according to a sin and cos formula
    // so a wavy flag effect occurs
	 float4 ret = tex2D(foreground, Tex.xy);
	 
	 if(ret.a != 0)
	 {
		Tex.y += (sin((Tex.x - boxLeft) * yFreq + (frameCounter * ySpeed)) * yAmplitude * pixelHeight);
		Tex.x += (cos((Tex.y - boxTop) * xFreq + (frameCounter * xSpeed)) * xAmplitude * pixelWidth);
		ret = tex2D(foreground, Tex.xy);
	 }
    return ret;
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
