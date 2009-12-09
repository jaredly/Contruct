// Outline
// Ashley Gullen
// PS 2.0
// Draw a line around the image.

//#CROSS-SAMPLING : reads pixels it may be writing.
//#BORDER-MODE : samples pixels outside bounding box
//#PARAM float thickness 1 : Thickness : Thickness of the outline.
float thickness;

// Foreground texture
texture ForegroundTexture;

// Foreground sampler
sampler2D foreground = sampler_state {
    Texture = (ForegroundTexture);
    MinFilter = Point;
    MagFilter = Point;
    MipFilter = Point;
};

float pixelWidth;
float pixelHeight;

// Effect function
float4 EffectProcess( float2 Tex : TEXCOORD0 ) : COLOR0
{
    float4 here = tex2D(foreground, Tex.xy);
    float4 left = tex2D(foreground, float2(Tex.x - pixelWidth * thickness, Tex.y));
    float4 right = tex2D(foreground, float2(Tex.x + pixelWidth * thickness, Tex.y));
    float4 top = tex2D(foreground, float2(Tex.x, Tex.y - pixelHeight * thickness));
    float4 bottom = tex2D(foreground, float2(Tex.x, Tex.y + pixelHeight * thickness));


    if (here.a == 0) {
        if (left.a != 0 || right.a != 0 || top.a != 0 || bottom.a != 0) {
            return float4(1,1,1,1);
        }
	else return float4(0,0,0,0);
    }
    else return here;

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
