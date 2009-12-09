// Mask
// Ashley Gullen
// PS 0.0
// Use a mask to hide or show sections of an image.

// ConstructEffect
technique ConstructEffect
{
    pass p0
    {
        VertexShader = null;
	PixelShader = null;
  	AlphaBlendEnable = TRUE;
  	SrcBlend         = ZERO;
  	DestBlend        = SRCALPHA;  
	AlphaTestEnable = FALSE;
    }
}
