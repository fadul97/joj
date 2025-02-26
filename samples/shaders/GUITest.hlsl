cbuffer ColorBuffer : register(b0)
{
    float4 rectColor;
};

struct VS_INPUT
{
    float2 PosL : POSITION;
};

struct VS_OUTPUT
{
    float4 PosH : SV_POSITION;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.PosH = float4(input.PosL, 0.0, 1.0f); // Keep position
    return output;
}

float4 PS() : SV_TARGET
{
    return rectColor;
}