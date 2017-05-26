#include "SharedVertexShaderData.vs"
#include "SharedPixelShaderData.ps"

TextureCube skybox;
//SamplerState samAnisotropic
//{
//	Filter = ANISOTROPIC;
//	MaxAnisotropy = 4;
//
//	AddressU = WRAP;
//	AddressV = WRAP;
//};

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float4 norm : NORMAL0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float4 norm : NORMAL0;
};

RasterizerState BackFaceCulling
{
	cullmode = none;
};

PixelInputType PBR_VS(VertexInputType input)
{
	PixelInputType output;

	output.position = input.position;
	output.position.w = 1;

	output.position = mul(output.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.tex = input.tex;
	output.norm = (input.norm);
	return output;
}

float4 PBR_PS(PixelInputType input) :SV_TARGET
{
	float4 color;

	color = albedoTexture.Sample(samAnisotropic, input.tex);
	//return float4(1,0,0,1);
	return color;
}

BlendState NoBlending
{
	AlphaToCoverageEnable = FALSE;
	BlendEnable[0] = FALSE;
};

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

technique11 Skybox
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, PBR_VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PBR_PS()));
		SetRasterizerState(BackFaceCulling);

		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetDepthStencilState(EnableDepth, 0);
	}
}