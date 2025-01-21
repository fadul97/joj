cbuffer CB2D : register(b0)
{
    float4x4 gWorldViewProj; // Matriz de transformação (mundo -> view -> projeção)
    float4 gColor;           // Cor multiplicadora (RGBA)
    float4 spriteUVOffset;   // Offset para coordenadas UV (x: minX, y: minY, z: maxX, w: maxY)
};

Texture2D resource;

SamplerState linearfilter
{
    Filter = MIN_MAG_MIP_LINEAR;
};

SamplerState anisotropic
{
    Filter = ANISOTROPIC;
    MaxAnisotropy = 4;
};

struct VS_INPUT
{
    float3 position : POSITION; // Posição do vértice
    float2 texCoord : TEXCOORD; // Coordenadas de textura
};

struct PS_INPUT
{
    float4 position : SV_POSITION; // Posição projetada na tela
    float2 texCoord : TEXCOORD;    // Coordenadas de textura interpoladas
};

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;

    // Transformar a posição do vértice
    float4 worldPosition = float4(input.position, 1.0f);
    output.position = mul(worldPosition, gWorldViewProj);

    // Ajustar coordenadas UV com base no retângulo UV fornecido
    output.texCoord = input.texCoord * (spriteUVOffset.zw - spriteUVOffset.xy) + spriteUVOffset.xy;

    return output;
}

float4 PS(PS_INPUT input) : SV_TARGET
{
    // Amostrar a textura na coordenada UV
    float4 texColor = resource.Sample(anisotropic, input.texCoord);

    // Aplicar multiplicação pela cor global
    float4 finalColor = texColor * gColor;

    // Transparência: descartar pixels com alfa baixo
    if (finalColor.a < 0.1f)
    {
        discard;
    }

    return finalColor;
}
