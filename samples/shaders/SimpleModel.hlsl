#include "LightHelper.hlsl"

cbuffer cameraCB : register(b0)
{
    float4x4 gWorld;
    float4x4 gInverseWorld;
    float4x4 gView;
    float4x4 gProj;
    float4x4 gViewProj;
    float4x4 gWVP;
    float3 gEyePosW;
};

cbuffer lightCB : register(b1)
{
    DirectionalLight gDirectionalLight;
};

struct VertexIn
{
    float3 PosL : POSITION;
    float4 Color : COLOR;
    float3 Normal : NORMAL;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float4 Color : COLOR;
    float3 Normal : NORMAL;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;

    // Transformar posi��o para o espa�o de recorte (clip space)
    vout.PosH = mul(float4(vin.PosL, 1.0f), gWVP);

    // Transformar a normal para o espa�o do mundo
    // Usamos apenas a matriz 3x3 de gWorld para a normal, ignorando a transla��o
    vout.Normal = normalize(mul((float3x3)gWorld, vin.Normal));

    // Passar a cor para o Pixel Shader
    vout.Color = vin.Color;

    return vout;
}


float4 PS(VertexOut pin) : SV_Target
{
    // return pin.Color;
    
    // Normalizar a normal interpolada
    pin.Normal = normalize(pin.Normal);

    // Inverter a dire��o da luz para garantir que a luz est� vindo da dire��o correta
    float3 L = -normalize(gDirectionalLight.Direction);

    // Calcular ilumina��o difusa (Lambert)
    float diff = max(dot(pin.Normal, L), 0.0f);

    // Calcular a ilumina��o especular (Phong)
    float3 V = normalize(gEyePosW - pin.PosH.xyz); // Vetor de vis�o
    float3 R = reflect(L, pin.Normal); // Vetor de reflex�o
    float spec = pow(max(dot(V, R), 0.0f), 32.0f); // Especular com um valor de "specular power" arbitr�rio (32)

    // Combinar luz ambiente, difusa e especular
    float4 ambient = gDirectionalLight.Ambient * pin.Color;
    float4 diffuse = gDirectionalLight.Diffuse * diff * pin.Color;
    float4 specular = gDirectionalLight.Specular * spec * pin.Color;

    // Finalizar o c�lculo de cor com todas as contribui��es de luz
    float4 finalColor = ambient + diffuse + specular;

    // Garantir que a cor est� dentro do limite de 0 a 1
    return saturate(finalColor);
}


