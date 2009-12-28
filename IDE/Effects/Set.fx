// Set
// David Clark
// PS 0.0
// Set the pixels rather than combine them.

// ConstructEffect
technique ConstructEffect
{
    pass p0
    {
       VertexShader = null;
		PixelShader = null;
		AlphaBlendEnable = TRUE;
		SrcBlend         = ONE;
		DestBlend        = ZERO;       
    }

}
