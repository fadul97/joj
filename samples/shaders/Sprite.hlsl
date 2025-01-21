cbuffer CB2D : register(b0)
{
    float4x4 gWorldViewProj; // Matriz de transformação (mundo -> view -> projeção)
    float4 gColor; // Cor multiplicadora (RGBA)
    float4 spriteUVOffset; // Offset para coordenadas UV (x: minX, y: minY, z: maxX, w: maxY)
};

Texture2D gTexture : register(t0); // Textura do sprite
SamplerState gSampler : register(s0); // Estado do sampler

struct VS_INPUT
{
    float3 position : POSITION; // Posição do vértice
    float2 texCoord : TEXCOORD; // Coordenadas de textura
};

struct PS_INPUT
{
    float4 position : SV_POSITION; // Posição projetada na tela
    float2 texCoord : TEXCOORD; // Coordenadas de textura interpoladas
};

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;
    
    // Transformar posição do vértice
    float4 worldPosition = float4(input.position, 1.0f);
    output.position = mul(worldPosition, gWorldViewProj);
    
    // Ajustar coordenadas UV com base no retângulo UV fornecido
    // output.texcoord = input.texcoord;
    output.texCoord = input.texCoord * (spriteUVOffset.zw - spriteUVOffset.xy) + spriteUVOffset.xy;
    
    return output;
}

float4 PS(PS_INPUT input) : SV_TARGET
{
    // return float4(1.0f, 0.0f, 0.0f, 1.0f);

    // Amostrar a textura na coordenada UV
    float4 texColor = gTexture.Sample(gSampler, input.texCoord);

    // Se a cor de alfa for 0, a parte do sprite será transparente (efeito alfa).
    if (texColor.a < 0.1f)  // Um valor baixo de alfa indica transparência.
    {
        discard; // Descartar o pixel, criando uma região transparente.
    }

    return texColor; // Retorna a cor do pixel.
}