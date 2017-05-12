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

float4 Fresnel(float4 substance, float3 l, float3 h)
{
	return substance + (1 - substance)*pow(dot(l, h), 5);
}

float3 RotateNormal(float3 normal, float3 rot_axis, float angle)
{

	float3x3 rot_matrix = 
	{	
		rot_axis.x*rot_axis.x*(1-cos(angle)) + cos(angle),
		rot_axis.y*rot_axis.x*(1-cos(angle)) + rot_axis.z*sin(angle),
		rot_axis.z*rot_axis.x*(1-cos(angle)) - rot_axis.y*sin(angle),

		rot_axis.x*rot_axis.y*(1-cos(angle)) - rot_axis.z*sin(angle),
		rot_axis.y*rot_axis.y*(1-cos(angle)) + cos(angle),
		rot_axis.z*rot_axis.y*(1-cos(angle)) + rot_axis.x*sin(angle),

		rot_axis.x*rot_axis.z*(1-cos(angle)) + rot_axis.y*sin(angle),
		rot_axis.y*rot_axis.z*(1-cos(angle)) + rot_axis.x*sin(angle),
		rot_axis.z*rot_axis.z*(1-cos(angle)) + cos(angle)
	};
	float3 new_normal = mul(normal, rot_matrix); 

	return new_normal;
}
static const float PI = 3.14159265f;
float4 PBR_PS(PixelInputType input) :SV_TARGET
{
	float4 albedo = albedoTexture.Sample(samAnisotropic, input.tex);
	float4 normal = normalize(normalTexture.Sample(samAnisotropic, input.tex));
	float roughness = roughnessTexture.Sample(samAnisotropic, input.tex).r;
	float4 substance = substanceTexture.Sample(samAnisotropic, input.tex);
	float AP = pow(8192, roughness);

	float3 light_dir = normalize(float3(cos(d_time), -0.5, sin(d_time)));
	float3 light_col = float3(.5, 0.5, 0.5);

	const float4 eyeVec = normalize(cam_pos - input.position);
	const float4 half_vec = (float4(light_dir,0) + eyeVec)/length(float4(light_dir,0) + eyeVec);

	float4 ambient = float4(0.9,0.9,0.9,1);
	float4 default_normal_m = input.norm;
	float normal_angle_diff = acos(dot(normal, default_normal_m)/(length(default_normal_m)*length(normal)));

	normal = float4(RotateNormal(normal.xyz, cross(normal.xyz, default_normal_m.xyz), normal_angle_diff), 0);
	normal = normalize(normal);
	float4 specular_light = float4(1,1,1,1);
	float HDotN = pow(saturate(dot(half_vec, normal)), AP);
	float NDotL = saturate(dot(normal, float4(light_dir,0)));
	float4 spec_highlight = ((roughness + 8)/(8)) * HDotN * NDotL;
	float4 finalColor = 0;
	finalColor += albedo * ambient;
	finalColor += float4(saturate(dot(light_dir, normal.xyz) *light_col), 0);
	finalColor += (spec_highlight);
	finalColor.a = 1;

	return finalColor;
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
		SetVertexShader(CompileShader(vs_5_0, PBR_VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PBR_PS()));
		SetRasterizerState(BackFaceCulling);

		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetDepthStencilState(EnableDepth, 0);
	}
}