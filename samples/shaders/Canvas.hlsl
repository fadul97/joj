// Vertex Shader
struct VS_INPUT
{
    float3 Pos : POSITION; // Posi��o do v�rtice
    float4 Color : COLOR; // Cor do v�rtice
};

struct PS_INPUT
{
    float4 PosH : SV_POSITION; // Posi��o clip-space
    float4 Color : COLOR; // Cor interpolada
};

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;
    output.PosH = float4(input.Pos, 1.0f); // Converte para coordenadas homog�neas
    output.Color = input.Color; // Passa a cor para o Pixel Shader
    return output;
}

float4 PS(PS_INPUT input) : SV_TARGET
{
    return input.Color; // Retorna a cor para o render target
}
