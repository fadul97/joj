// matriz de transformação e projeção
cbuffer ConstantBuffer
{
    float4x4 WorldViewProj;
}

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

// estrutura dos vértices de entrada
struct VertexIn
{
    float3 Pos : POSITION;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD;
};

// estrutura dos vértices de saída
struct VertexOut
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD;
};

// programa principal do vertex shader
VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;

    // transforma vértices para coordenadas da tela
    vOut.Pos = mul(float4(vIn.Pos, 1.0f), WorldViewProj);
    
    // mantém as cores inalteradas
    vOut.Color = vIn.Color;

    // mantém as coordenadas da textura inalteradas
    vOut.Tex = vIn.Tex;

    return vOut;
}

float4 PS(VertexOut pIn) : SV_TARGET
{
    return resource.Sample(linearfilter, pIn.Tex) * pIn.Color;
}