TextureCube txSkyColour : register(t2);
SamplerState txSkySampler : register (s1);

cbuffer ConstantBuffer : register(b2)
{
	matrix World;
	matrix View;
	matrix Projection;
	float4 eyePosition;
}


struct VS_INPUT
{
	float4 Pos : POSITION;
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD0;
};

struct PS_INPUT_Cubemap
{
	float4 Pos : SV_POSITION;
	float3 Norm : TEXCOORD0;
	float2 Tex : TEXCOORD1;
	float3 viewDir : TEXCOORD2;
};
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT_Cubemap VS_main(VS_INPUT input)
{
	PS_INPUT_Cubemap output = (PS_INPUT_Cubemap)0;
	float4 inPos = input.Pos;
	inPos.xyz *= 100.0;
	output.viewDir = inPos.xyz;
	float4 skyPos = inPos;
	skyPos.xyz += eyePosition;//sky position to clipping space
	output.Pos = mul(skyPos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS_main(PS_INPUT_Cubemap input) : SV_Target
{
	return txSkyColour.Sample(txSkySampler, input.viewDir);
}