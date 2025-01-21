cbuffer CB2D : register(b0)
{
    float4x4 gWorldViewProj; // Matriz de transforma��o (mundo -> view -> proje��o)
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
    float3 position : POSITION; // Posi��o do v�rtice
    float2 texCoord : TEXCOORD; // Coordenadas de textura
};

struct PS_INPUT
{
    float4 position : SV_POSITION; // Posi��o projetada na tela
    float2 texCoord : TEXCOORD;    // Coordenadas de textura interpoladas
};

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;

    // Transformar a posi��o do v�rtice
    float4 worldPosition = float4(input.position, 1.0f);
    output.position = mul(worldPosition, gWorldViewProj);

    // Ajustar coordenadas UV com base no ret�ngulo UV fornecido
    output.texCoord = input.texCoord * (spriteUVOffset.zw - spriteUVOffset.xy) + spriteUVOffset.xy;

    return output;
}

float4 PS(PS_INPUT input) : SV_TARGET
{
    // Amostrar a textura na coordenada UV
    float4 texColor = resource.Sample(anisotropic, input.texCoord);

    // Aplicar multiplica��o pela cor global
    float4 finalColor = texColor * gColor;

    // Transpar�ncia: descartar pixels com alfa baixo
    if (finalColor.a < 0.1f)
    {
        discard;
    }

    return finalColor;
}
