cbuffer CB2D : register(b0)
{
    float4x4 gWorldViewProj; // Matriz de transformação (mundo -> view -> projeção)
    float4 gColor; // Cor multiplicadora (RGBA)
    float4 gUVRect; // Coordenadas UV (x, y, width, height)
};

Texture2D gTexture : register(t0); // Textura do sprite
SamplerState gSampler : register(s0); // Estado do sampler

struct VS_INPUT
{
    float3 position : POSITION; // Posição do vértice
    float2 texcoord : TEXCOORD; // Coordenadas de textura
};

struct PS_INPUT
{
    float4 position : SV_POSITION; // Posição projetada na tela
    float2 texcoord : TEXCOORD; // Coordenadas de textura interpoladas
};

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;
    
    // Transformar posição do vértice
    float4 worldPosition = float4(input.position, 1.0f);
    output.position = mul(worldPosition, gWorldViewProj);
    
    // Ajustar coordenadas UV com base no retângulo UV fornecido
    output.texcoord = input.texcoord;

    return output;
}

float4 PS(PS_INPUT input) : SV_TARGET
{
    // return float4(1.0f, 0.0f, 0.0f, 1.0f);

    // Amostrar a textura na coordenada UV
    float4 textureColor = gTexture.Sample(gSampler, input.texcoord);

    // Multiplicar a cor da textura pela cor fornecida
    float4 finalColor = textureColor * gColor;

    return finalColor;
}