#include "SharedPixelShaderData.ps"
#include "SharedVertexShaderData.vs"

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
	cullmode = back;
};

float4 TexturePixelShader(PixelInputType input) :SV_TARGET
{
	float4 textureColor;
	textureColor = shaderTexture.Sample(samAnisotropic, input.tex);
	
	float3 light_dir = float3(1, 0, -1);

	float dir_light = dot(normalize(float3(input.norm.xyz)), normalize(light_dir));
	dir_light = clamp(dir_light, 0.5, 1);
	return textureColor*dir_light;
}

PixelInputType TextureVertexShader(VertexInputType input)
{
	PixelInputType output;

	output.position = input.position;

	output.position = mul(output.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.tex = input.tex;
	output.norm = mul(input.norm, worldMatrix);
	return output;
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

technique11 Test
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, TextureVertexShader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, TexturePixelShader()));
		SetRasterizerState(BackFaceCulling);

		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetDepthStencilState(EnableDepth, 0);
	}
}