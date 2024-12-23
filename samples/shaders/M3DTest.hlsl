//=============================================================================
// NormalMap.fx by Frank Luna (C) 2011 All Rights Reserved.
//=============================================================================

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
    bool gUseTexure;
    bool gAlphaClip;
    bool gFogEnabled;
    bool gReflectionEnabled;
}; 

// Nonnumeric values cannot be added to a cbuffer.
Texture2D gDiffuseMap : register(t0);
Texture2D gNormalMap : register(t1);

SamplerState samLinear : register(s0);
 
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
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float3 TangentW : TANGENT;
    float2 Tex : TEXCOORD0;
    float4 ShadowPosH : TEXCOORD1;
    float4 SsaoPosH : TEXCOORD2;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
	
	// Transform to world space space.
    vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
    vout.NormalW = mul(vin.NormalL, (float3x3) gWorldInvTranspose);
    vout.TangentW = mul(vin.TangentL, (float3x3) gWorld);

	// Transform to homogeneous clip space.
    vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	
	// Output vertex attributes for interpolation across triangle.
    vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;

	// Generate projective tex-coords to project shadow map onto scene.
    vout.ShadowPosH = mul(float4(vin.PosL, 1.0f), gShadowTransform);

	// Generate projective tex-coords to project SSAO map onto scene.
    vout.SsaoPosH = mul(float4(vin.PosL, 1.0f), gWorldViewProjTex);

    return vout;
}
 
float4 PS(VertexOut pin) : SV_Target
{
    // return float4(1, 0, 0, 1);
	
	// Interpolating normal can unnormalize it, so normalize it.
    pin.NormalW = normalize(pin.NormalW);

	// The toEye vector is used in lighting.
    float3 toEye = gEyePosW - pin.PosW;

	// Cache the distance to the eye from this surface point.
    float distToEye = length(toEye);

	// Normalize.
    toEye /= distToEye;
	
    // Default to multiplicative identity.
    float4 texColor = float4(1, 1, 1, 1);

    if (gUseTexure)
    {
		// Sample texture.
        texColor = gDiffuseMap.Sample(samLinear, pin.Tex);

        if (gAlphaClip)
        {
			// Discard pixel if texture alpha < 0.1.  Note that we do this
			// test as soon as possible so that we can potentially exit the shader 
			// early, thereby skipping the rest of the shader code.
            clip(texColor.a - 0.1f);
        }
		
    }

    // return texColor;
	
	//
	// Normal mapping
	//

    float3 normalMapSample = gNormalMap.Sample(samLinear, pin.Tex).rgb;
	 
	//
	// Lighting.
	//

    float4 litColor = texColor;

    return litColor;
}