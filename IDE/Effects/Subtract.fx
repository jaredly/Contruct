// Subtractive
// David Clark
// PS 0.0
// Simple subtractive blend.

// ConstructEffect
technique ConstructEffect
{

    pass p0
    {
        VertexShader = null;
   	PixelShader = null;
  	AlphaBlendEnable = TRUE;
  	SrcBlend         = ZERO;
  	DestBlend        = INVSRCCOLOR;       
    }


}
