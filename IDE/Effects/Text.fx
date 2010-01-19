// Text
// David Clark
// PS 0.0
// Simple blend that makes dark text look better.

// ConstructEffect
technique ConstructEffect
{
    pass p0
    {
		VertexShader = null;
		PixelShader = null;
		AlphaBlendEnable = TRUE;
		SrcBlend         = SRCALPHA;
		DestBlend        = INVSRCALPHA;       
    }

}
