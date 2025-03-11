#ifndef _JOJ_D3D11_SHADER_LIBRARY_H
#define _JOJ_D3D11_SHADER_LIBRARY_H

#include "core/defines.h"

namespace joj
{
	namespace D3D11ShaderLibrary
	{
        // FIXME: Include needs to be in an appropriate folder path
        const char* VertexShaderCode = R"(
            #include "../../../../samples/shaders/LightHelper.hlsl"

            cbuffer ConstantBuffer : register(b0) {
                float4x4 wvp;
                float4x4 worldMatrix;
	            float4x4 viewMatrix;
	            float4x4 projectionMatrix;
            };

            cbuffer CameraBuffer : register(b1)
            {
                float3 cameraPosition;
                float padding;
            };

            struct VS_INPUT {
                float4 position : POSITION;
                float2 tex : TEXCOORD0;
                float3 normal : NORMAL;
            };

            struct PS_INPUT {
                float4 position : SV_POSITION;
                float2 tex : TEXCOORD0;
                float3 normal : NORMAL;
                float3 viewDirection : TEXCOORD1;
            };

            PS_INPUT main(VS_INPUT input) {
                PS_INPUT output;
                float4 worldPosition;
        
                // Change the position vector to be 4 units for proper matrix calculations.
                input.position.w = 1.0f;

                // Calculate the position of the vertex against the world, view, and projection matrices.
                output.position = mul(input.position, worldMatrix);
                output.position = mul(output.position, viewMatrix);
                output.position = mul(output.position, projectionMatrix);

                output.tex = input.tex;

                // Calculate the normal vector against the world matrix only.
                output.normal = mul(input.normal, (float3x3)worldMatrix);

                // Normalize the normal vector.
                output.normal = normalize(output.normal);

                // Calculate the position of the vertex in the world.
                worldPosition = mul(input.position, worldMatrix);

                // Determine the viewing direction based on the position of the camera and the position of the vertex in the world.
                output.viewDirection = cameraPosition.xyz - worldPosition.xyz;

                // Normalize the viewing direction vector.
                output.viewDirection = normalize(output.viewDirection);

                return output;
            }
        )";

        const char* PixelShaderCode = R"(
            cbuffer LightBuffer : register(b0)
            {
                float4 ambientColor;
                float4 diffuseColor;
                float3 lightDirection;
                float specularPower;
                float4 specularColor;
            };

            struct PS_INPUT {
                float4 position : SV_POSITION;
                float2 tex : TEXCOORD0;
                float3 normal : NORMAL;
                float3 viewDirection : TEXCOORD1;
            };

            Texture2D shaderTexture : register(t0);
            SamplerState SampleType : register(s0);

            float4 main(PS_INPUT input) : SV_TARGET {
                float4 textureColor;
                float3 lightDir;
                float lightIntensity;
                float4 color;
                float3 reflection;
                float4 specular;

                // Sample the pixel color from the texture using the sampler at this texture coordinate location.
                // textureColor = shaderTexture.Sample(SampleType, input.tex);

                // Set the default output color to the ambient light value for all pixels.
                color = ambientColor;

                // Initialize the specular color.
                specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
                // specular = specularColor;

                // Invert the light direction for calculations.
                lightDir = -lightDirection;

                // Calculate the amount of light on this pixel.
                lightIntensity = saturate(dot(input.normal, lightDir));

                if(lightIntensity > 0.0f)
                {
                    // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
                    color += (diffuseColor * lightIntensity);

                    // Saturate the ambient and diffuse color.
                    color = saturate(color);

                    // Calculate the reflection vector based on the light intensity, normal vector, and light direction.
                    reflection = normalize(2.0f * lightIntensity * input.normal - lightDir);

                    // Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
                    // specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);

                    // FIXED? Multiply specular lighting by specularColor (not being done). -> 'Dot' Light now has color to it.
                    specular = specularColor * pow(saturate(dot(reflection, input.viewDirection)), specularPower);
                }

                // Multiply the texture pixel and the final diffuse color to get the final pixel color result.
                // color = color * textureColor;

                // Add the specular component last to the output color.
                color = saturate(color + specular);

                return color;
            }
        )";

        const char* VertexShaderCanvas = R"(
            struct VS_INPUT
            {
                float3 PosL : POSITION;  // Posi��o local
                float4 ColorL : COLOR;   // Cor local
            };

            struct PS_INPUT
            {
                float4 PosH : SV_POSITION; // Posi��o clip-space
                float4 ColorH : COLOR;     // Cor para o shader de pixels
            };

            PS_INPUT VS(VS_INPUT input) {
                PS_INPUT output;

                // Converter para homogeneizar a coordenada PosH
                output.PosH = float4(input.PosL, 1.0f); // Adiciona w = 1.0f
                output.ColorH = input.ColorL;

                return output;
            }
        )";

        const char* PixelShaderCanvas = R"(
            struct PS_INPUT {
                float4 position : SV_POSITION; // Posi��o clip-space
                float4 color : COLOR;          // Cor recebida do VS
            };

            float4 PS(PS_INPUT input) : SV_TARGET {
                return input.color; // Retorna a cor para o render target
            }
        )";
	}
}

#endif // _JOJ_D3D11_SHADER_LIBRARY_H