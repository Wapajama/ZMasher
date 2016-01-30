#include "SharedPixelShaderData.ps"
#include "SharedVertexShaderData.vs"

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 TexturePixelShader(PixelInputType input) :SV_TARGET
{
	float4 textureColor;
	textureColor = shaderTexture.Sample(samAnisotropic, input.tex);
	return textureColor;
}

PixelInputType TextureVertexShader(VertexInputType input)
{
	PixelInputType output;

	input.position.w = 1.f;//"WE NEED THIS" :s

	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.tex = input.tex;

	return output;
}


technique11 Test
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, TextureVertexShader()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, TexturePixelShader()));
	}
}