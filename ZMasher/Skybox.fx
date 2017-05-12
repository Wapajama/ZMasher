#include "SharedPixelShaderData.ps"
#include "SharedVertexShaderData.vs"

#pragma region GENERAL_EXTRACTME

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
	output.norm = mul(input.norm, worldMatrix);
	return output;
}
#pragma endregion


