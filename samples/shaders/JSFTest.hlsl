#include "LightHelper.hlsl"
 
cbuffer cbPerFrame : register(b1)
{
    DirectionalLight gDirLights[3];
    float3 gEyePosW;

    float gFogStart;
    float gFogRange;
    float4 gFogColor;
};

cbuffer cbPerObject : register(b0)
{
    float4x4 gWorld;
    float4x4 gWorldInvTranspose;
    float4x4 gWorldViewProj;
    float4x4 gWorldViewProjTex;
    float4x4 gTexTransform;
    float4x4 gShadowTransform;
    Material gMaterial;
    bool gUseTexture;
    bool gAlphaClip;
    bool gFogEnabled;
    bool gReflectionEnabled;
}; 

Texture2D gDiffuseMap : register(t0);
Texture2D gNormalMap : register(t1);

SamplerState samLinear : register(s0);

SamplerComparisonState samShadow
{
    Filter = COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
    AddressU = BORDER;
    AddressV = BORDER;
    AddressW = BORDER;
    BorderColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

    ComparisonFunc = LESS_EQUAL;
};

struct VertexIn
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float2 Tex : TEXCOORD;
    float3 TangentL : TANGENT;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float2 TexC : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
	
	// Transform to homogeneous clip space.
    vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	
    vout.TexC = vin.Tex;
    
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    if (gUseTexture == 1)
    {
        return gDiffuseMap.Sample(samLinear, pin.TexC);
    }
    
    return float4(1, 0, 0, 1);
}
