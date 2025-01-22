cbuffer CB2D : register(b0)
{
    float4x4 gWorldViewProj; // Matriz de transformação (mundo -> view -> projeção)
    float4 gColor;
};

struct VS_INPUT
{
    float3 position : POSITION; // Posição do vértice
    float4 color : COLOR; // Coordenadas de textura
};

struct PS_INPUT
{
    float4 position : SV_POSITION; // Posição projetada na tela
    float4 color : COLOR; // Coordenadas de textura interpoladas
};

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;

    // Transformar a posição do vértice
    float4 worldPosition = float4(input.position, 1.0f);
    output.position = mul(worldPosition, gWorldViewProj);

    // Ajustar coordenadas UV com base no retângulo UV fornecido
    output.color = gColor;

    return output;
}

float4 PS(PS_INPUT input) : SV_TARGET
{
    return input.color;
}
