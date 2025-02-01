cbuffer cameraCB : register(b0)
{
    float4x4 gView;
    float4x4 gProj;
    float4x4 gViewProj;
    float4x4 gWVP;
    float3 gEyePosW;
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
	
	// Transform to homogeneous clip space.
    // vout.PosH = float4(vin.PosL, 1.0);
    vout.PosH = mul(float4(vin.PosL, 1.0f), gWVP);
    
    // Pass normal to pixel shader.
    vout.Normal = vin.Normal;
    
    // Pass color to pixel shader.
    vout.Color = vin.Color;
	
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    // Interpolating normal can unnormalize it, so normalize it.
    pin.Normal = normalize(pin.Normal);
    
    // The toEye vector is used in lighting.
    float3 toEye = gEyePosW - float3(pin.PosH.x, pin.PosH.y, pin.PosH.z);
    toEye = normalize(toEye);

	// Cache the distance to the eye from this surface point.
    float distToEye = length(toEye);
    
    float3 lightDir = float3(0.0f, -1.0f, 0.0f); // Luz vindo de cima
    float3 lightColor = float3(1.0f, 1.0f, 1.0f); // Cor branca
    
    // Cálculo de iluminação difusa (Lambertian)
    float diff = max(dot(pin.Normal, -lightDir), 0.0f); // Produto escalar da normal e direção da luz
    
    // Cálculo de iluminação especular (Phong)
    float3 reflectDir = reflect(lightDir, pin.Normal); // Reflexão da luz sobre a normal
    float spec = pow(max(dot(reflectDir, toEye), 0.0f), 32); // Reflexão especular, coeficiente de brilho = 32
    
    // Combinando a iluminação difusa e especular
    float4 diffuse = float4(lightColor, 1.0f) * diff * pin.Color; // Difusa
    float4 specular = float4(lightColor, 1.0f) * spec; // Especular
    
    // Somar as duas contribuições de iluminação
    return diffuse + specular;
}
