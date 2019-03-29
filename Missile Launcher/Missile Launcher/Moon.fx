////--------------------------------------------------------------------------------------
//// File: Tutorial04.fx
////
//// Copyright (c) Microsoft Corporation. All rights reserved.
////--------------------------------------------------------------------------------------
//
////--------------------------------------------------------------------------------------
//// Constant Buffer Variables
////--------------------------------------------------------------------------------------
//Texture2D txStoneColor : register(t0);
//Texture2D txStoneBump : register(t1);
//SamplerState txStoneSampler : register (s0);
//
//cbuffer ConstantBuffer : register(b3)
//{
//	matrix World;
//	matrix View;
//	matrix Projection;
//	float4 eyePosition;
//	float4 lightPosition;
//}
//
////--------------------------------------------------------------------------------------
//
//struct VS_INPUT
//{
//	float4 Pos : POSITION;
//	float3 Normal : NORMAL;
//
//};
//
//struct VS_OUTPUT
//{
//	float4 Pos :			 SV_POSITION;
//	float3 ViewDirection :   TEXCOORD1;
//	float3 LightDirection :  TEXCOORD2;
//	float3 Normal :          TEXCOORD3;
//};
//
////--------------------------------------------------------------------------------------
//// Vertex Shader
////--------------------------------------------------------------------------------------
//VS_OUTPUT VS_main(VS_INPUT input)
//{
//	VS_OUTPUT output = (VS_OUTPUT)0;
//	output.Pos = mul(input.Pos, World);
//	output.Pos = mul(output.Pos, View);
//	output.Pos = mul(output.Pos, Projection);
//
//
//	float3 fvObjectPosition = mul(input.Pos, View);
//
//	output.ViewDirection = -fvObjectPosition;
//	output.LightDirection = mul(lightPosition - input.Pos, View);
//
//	output.Normal = mul(input.Normal, View);
//
//	return(output);
//
//
//}
//
//
////--------------------------------------------------------------------------------------
//// Pixel Shader
////--------------------------------------------------------------------------------------
//float4 PS_main(VS_OUTPUT input) : SV_Target
//{
//
//	//Problem with the specular lighting
//	float specularPower = 30.0f;
///*=======================================================*/
//float4 ambientColour = float4(0.15f, 0.15f, 0.15f, 0.15f);
///*=======================================================*/
//float4 diffuseColour = float4(1.0f, 1.0f, 1.0f, 1.0f);
////float3 lightDirection = float3(0.15f, 0.0f, 1.0f); /// GET LECTURER TO CHECK Y
//
////float4 specular = float4(1.0f, 1.0f, 1.0f, 1.0f);
//
//float4 colour = ambientColour;
//float3 lightDir = input.LightDirection;
//float3 N = normalize(input.Normal);
//float3 L = normalize(lightDir);
//float diff = max(0.0, dot(N, L));
//
//
//float3 R = reflect(-L, N);
//R = normalize(R);
//
//float3 V = normalize(input.ViewDirection);
//float  fRDotV = max(0.0f, dot(R, V));
//float spec = float4(1.0f,1.0f,1.0f,1.0f) * pow(fRDotV, 5);;
//
////if (lightIntensity > 0.0f)
////{
////	colour += (diffuseColour * lightIntensity);
////	colour = saturate(colour);
////	float3 reflection = normalize(2 * lightIntensity*input.Normal - lightDir);
////	specular = pow(saturate(dot(reflection, input.ViewDirection)), specularPower);
////}
////else
////{
////	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
////}
//
////colour = saturate(colour);
//
//colour = saturate(colour + spec);
//
////float4 materialAmb = float4(0.0, 0.0, 0.0, 1.0); 
////float4 lightCol = float4(1.0, 1.0, 1.0, 1.0);  
//
////float3 lightVec = normalize(input.lightVec);
////float3 normal = normalize(input.Norm); 
////
//////float diff = clamp(dot(lightVec, float3(0,1,0)),0.0f,1.0f);
////float diff = clamp(dot(lightVec, normal),0.0f,1.0f);
////float4 materialDiff = float4(0.9, 0.7, 1.0, 1.0);  
////float4 specular = 0;
//
////if (diff > 0.0f)
////{
////	float3 viewVec = normalize(input.viewVec);
////	float3 halfVec = normalize(lightVec - viewVec);
////	specular = pow(max(0.0, dot(halfVec, normal)), 1.0f);
////}
//////float3 outputColour = materialAmb  * materialDiff + diff * lightCol;
//////float4 outputColour = (materialAmb + diff * specular) *lightCol;
//
////float3 outputColour = materialAmb + lightCol * diff + lightCol * specular;
////float4 woodColour = txWoodColour.SampleLevel(txWoodSampler, 1 * input.Tex, 1);
//float4 finalCol = (float4)1.0;
//finalCol.rgb = 0.1*ambientColour + 0.5*diff *diffuseColour.rgb + spec;
////float4 light = (materialAmb + diff * diff) *lightCol;
//return finalCol;
//}
//--------------------------------------------------------------------------------------
// File: Tutorial04.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register(b5)
{
	matrix World;
	//matrix WorldM;
	matrix View;
	matrix Projection;
}

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 Pos : POSITION;
	float4 Color : COLOR;
};

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS_main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(input.Pos, World);
	//output.Pos = mul(output.Pos, WorldM);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Color = float4 (0.5f, 0.5f, 0.5, 0.0);
	return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS_main(VS_OUTPUT input) : SV_Target
{
	return input.Color;
}
