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
    vout.PosH = float4(vin.PosL, 1.0);
    
    // Pass color to pixel shader.
    vout.Color = vin.Color;
	
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    return pin.Color;
}
