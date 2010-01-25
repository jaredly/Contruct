// Bumpmapping
// Ashley Gullen
// PS 2.0

// Foreground texture
texture ForegroundTexture;

// Background texture
texture BackgroundTexture;

//#PARAM float lightx 0 : Light X : X co-ordinate of the light.
float lightx;

//#PARAM float lighty 0 : Light Y : Y co-ordinate of the light.
float lighty;

//#PARAM float lightz 0.1 : Light Z : Z co-ordinate of the light.
float lightz;

//#PARAM float intensity 2.5 : Intensity : Intensity of light.
float intensity;

float pixelWidth;
float pixelHeight;

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
    float4 normalmap = tex2D(foreground, Tex.xy);
	 if( normalmap.a != 0 )
	 {
		 float4 back = tex2D(background, Tex.xy);
		 float3 normal = 2.0f * normalmap.rgb - 1.0f;
		 float3 light = normalize(float3(lightx - Tex.x, lighty - Tex.y, lightz));
		 float diffuse = saturate(dot(normal, light)); 
		 normalmap = intensity * back * diffuse;
	 }
	 return normalmap;
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
