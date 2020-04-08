#include "SharedPixelShaderData.ps"
#include "SharedVertexShaderData.vs"

BlendState FillTechBlend
{  
	AlphaToCoverageEnable = FALSE;  
	BlendEnable[0] = TRUE;  
	SrcBlend = SRC_ALPHA;  
	DestBlend = INV_SRC_ALPHA;  
	BlendOp = ADD;  
	SrcBlendAlpha = ONE;  
	DestBlendAlpha = ZERO;  
	BlendOpAlpha = ADD;  
	RenderTargetWriteMask[0] = 0x0F;  
};
DepthStencilState FillTechDepth  
{  
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};
RasterizerState BackFaceCulling
{
	cullmode = NONE;
	//ScissorEnable = true;
	//AntialiasedLineEnable = true;
};

struct VSIn
{
	float4 position : POSITION;
	float4 colour : COLOR;
};

struct VSOut  
{  
	float4 position : SV_POSITION;  
	float4 colour : COLOR;
};   
VSOut VS(VSIn input)
{  
	VSOut output;

	output.position = input.position;
	output.position = output.position;
	//output.position.w = 0;

	output.position = mul(output.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	//output.position.x = clamp(output.position.x, -10, 10);
	//output.position.x = 100;
	//output.position.y = clamp(output.position.y, -10000, 10000);
	output.position.z = 0;
	//output.position.w = clamp(output.position.w, -10, 10);
	//output.position.z = 0;

	output.colour = input.colour;
	// output.colour.b = 1.f;
	// output.colour.g = 1.f;
	// output.colour.r = 1.f;
	// output.colour.a = 1.f;
	return output;  
}

float4 PS(VSOut input) : SV_TARGET  
{  
	return input.colour;  
}
technique11 FillTech  
{  
	pass p0  
	{  
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
		SetRasterizerState(BackFaceCulling);
		SetBlendState(FillTechBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetDepthStencilState(FillTechDepth, 0);
	}  
}  ;