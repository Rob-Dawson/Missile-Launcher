//--------------------------------------------------------------------------------------
// File: Tutorial04.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D txStoneColor : register(t0);
Texture2D txStoneBump : register(t1);
SamplerState txStoneSampler : register (s0);

cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix WorldM;
	matrix View;
	matrix Projection;
	float4 eyePosition;
	float4 lightPosition[2];
}
//
//cbuffer LightPositionBuffer
//{
//	float4 lightPosition[2];
//};

//--------------------------------------------------------------------------------------

struct VS_INPUT
{
	float4 Pos					: POSITION;
	float2 Tex					: TEXCOORD;
	float3 Norm					: NORMAL;
	float3 Tang					: TANGENT;
	float3 Binorm				: BINORMAL;
};

struct VS_OUTPUT
{
	float4 Pos					: SV_POSITION;
	float2 Tex					: TEXCOORD0;
	float3 viewDirInTang		: TEXCOORD1;
	float3 lightDirInTang		: TEXCOORD2;
	float3 lightDirInTang2		: TEXCOORD3;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS_main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, WorldM);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Tex = input.Tex;

	float3 viewDirW = eyePosition - input.Pos;

	float3 lightDirW = lightPosition[0] - input.Pos;
	float3 lightDirW1 = lightPosition[1] - input.Pos;

	float3 N = normalize(input.Norm);
	float3 T = normalize(input.Tang);
	float3 B = normalize(input.Binorm);

	float3x3 mat2Tang = float3x3 (T, B, N);
	output.viewDirInTang = mul(mat2Tang, viewDirW);
	output.lightDirInTang = mul(mat2Tang, lightDirW);
	output.lightDirInTang2 = mul(mat2Tang, lightDirW1);

	return output;

}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS_main(VS_OUTPUT input) : SV_Target
{
	float4 finalColor = 0;
	float4 stoneCol = txStoneColor.Sample(txStoneSampler, input.Tex);
	float4 stoneNormal = txStoneBump.Sample(txStoneSampler, input.Tex);

	//float diffuse = saturate(dot(lightDir, normal))

	float3 N = normalize(2.0* stoneNormal.xyz - 1.0);
	float3 L = normalize(input.lightDirInTang);

	float3 N2 = normalize(2.0* stoneNormal.xyz - 1.0);
	float3 L2 = normalize(input.lightDirInTang2);
	

	float dotLN = dot(L, N);
	float dotLN2 = dot(L2, N2);
	finalColor = stoneCol + stoneNormal;
	
	return (dotLN + dotLN2 ) * stoneCol;
}
