cbuffer CB2D : register(b0)
{
    float4x4 gWorldViewProj; // Matriz de transforma��o (mundo -> view -> proje��o)
    float4 gColor;
};

struct VS_INPUT
{
    float3 position : POSITION; // Posi��o do v�rtice
    float4 color : COLOR; // Coordenadas de textura
};

struct PS_INPUT
{
    float4 position : SV_POSITION; // Posi��o projetada na tela
    float4 color : COLOR; // Coordenadas de textura interpoladas
};

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;

    // Transformar a posi��o do v�rtice
    float4 worldPosition = float4(input.position, 1.0f);
    output.position = mul(worldPosition, gWorldViewProj);

    // Ajustar coordenadas UV com base no ret�ngulo UV fornecido
    output.color = gColor;

    return output;
}

float4 PS(PS_INPUT input) : SV_TARGET
{
    return input.color;
}
