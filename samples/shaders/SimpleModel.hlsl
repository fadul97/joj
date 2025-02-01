cbuffer cameraCB : register(b0)
{
    float4x4 gView;
    float4x4 gProj;
    float4x4 gViewProj;
    float4x4 gWVP;
    float3 gEyePosW;
};

struct VertexIn
{
    float3 PosL : POSITION;
    float4 Color : COLOR;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float4 Color : COLOR;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
	
	// Transform to homogeneous clip space.
    // vout.PosH = float4(vin.PosL, 1.0);
    vout.PosH = mul(float4(vin.PosL, 1.0f), gWVP);
    
    // Pass color to pixel shader.
    vout.Color = vin.Color;
	
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    return pin.Color;
}
