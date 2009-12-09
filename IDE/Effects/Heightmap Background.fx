// Height Map Background
// Ashley Gullen
// PS 2.0
// A glassy zoom on the background.

//#CROSS-SAMPLING : changes Tex.xy.
// Foreground texture
texture ForegroundTexture;

// Background texture
texture BackgroundTexture;

//#PARAM percent depth_intensity 1 : Depth Intensity: Depth intensity effect.
float depth_intensity;

//#PARAM percent specular 0.8 : specular: Determines how much light glare appears on flat surfaces.
float specular;

//#PARAM percent intensity 2.5 : Intensity : Intensity of the effect.
float intensity;

//#PARAM float lightx 0.5 : Light X : X co-ordinate of the light.
float lightx;

//#PARAM float lighty 0.5 : Light Y : Y co-ordinate of the light.
float lighty;

//#PARAM float lightz 0.1 : Light Z : Z co-ordinate of the light.
float lightz;

float pixelWidth;
float pixelHeight;

float hotspotX;
float hotspotY;


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

// Effect function
float4 EffectProcess( float2 Tex : TEXCOORD0 ) : COLOR0
{
    // Find the average of the RGB and set all components to that value
    // creating a greyscale image
   float4 normalmap;
	
	float c1 = tex2D(background, Tex).r;
	float c2 = tex2D(background, Tex + float2(pixelWidth,0)).r;
	float c3 = tex2D(background, Tex + float2(0, pixelHeight)).r;

	normalmap.r = 0.5 + (c1 - c2) * depth_intensity;
	normalmap.g = 0.5 + (c3 - c1) * depth_intensity;
	normalmap.b = specular;
   normalmap.a = 1.0f;


    // Screen formula
    float4 back = tex2D(background, Tex.xy);

    float3 normal = 2.0f * normalmap.rgb - 1.0f;

    float3 light = normalize(float3(hotspotX - Tex.x  , Tex.y - hotspotY, lightz));

    float diffuse = saturate(dot(normal, light));
    
    return intensity * back * diffuse * tex2D(foreground, Tex);
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
