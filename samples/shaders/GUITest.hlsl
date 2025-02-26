cbuffer ColorBuffer : register(b0)
{
    float4 rectColor;
};

struct VS_INPUT
{
    float2 pos : POSITION;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.pos = float4(input.pos, 0.0f, 1.0f); // Keep position
    return output;
}

float4 PS() : SV_TARGET
{
    return rectColor;
}