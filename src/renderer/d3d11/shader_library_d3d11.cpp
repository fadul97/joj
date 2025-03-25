#include "joj/renderer/shader_library.h"

#if JOJ_PLATFORM_WINDOWS

namespace joj
{
	namespace ShaderLibrary
	{
        /**
         * @brief A basic vertex shader that only passes the position and color
         * and only returns the color.
         */
        const char* SimpleColor = R"(
            struct VS_INPUT
            {
                float3 PosL : POSITION;
                float4 ColorL : COLOR;
            };

            struct PS_INPUT
            {
                float4 PosH : SV_POSITION;
                float4 ColorH : COLOR;
            };

            PS_INPUT VS(VS_INPUT input) {
                PS_INPUT output;

                output.PosH = float4(input.PosL, 1.0f);
                output.ColorH = input.ColorL;

                return output;
            }

            float4 PS(PS_INPUT input) : SV_TARGET {
                return input.ColorH;
            }
        )";

        /**
         * @brief A basic vertex shader that passes the position, color and normal.
         * It has a simple camera that transforms the vertices, the constant buffer
         * only holds a float4x4 matrix.
         */
        const char* PosColorNormalAndCamera = R"(
            cbuffer CB : register(b0)
            {
                float4x4 gWVP;
            };

            struct VS_INPUT
            {
                float3 PosL : POSITION;
                float4 ColorL : COLOR;
                float3 NormalL : NORMAL;
            };

            struct PS_INPUT
            {
                float4 PosH : SV_POSITION;
                float4 ColorH : COLOR;
                float3 NormalW : NORMAL;
            };

            PS_INPUT VS(VS_INPUT input) {
                PS_INPUT output;

                output.PosH = mul(float4(input.PosL, 1.0f), gWVP);
                output.ColorH = input.ColorL;
                output.NormalW = input.NormalL;

                return output;
            }

            float4 PS(PS_INPUT input) : SV_TARGET {
                return input.ColorH;
            }
        )";

        const char* LightWithPosColorNormalAndCamera = R"(
            cbuffer CB : register(b0)
            {
                float4x4 gWVP;
            };

            struct VS_INPUT
            {
                float3 PosL : POSITION;
                float4 ColorL : COLOR;
                float3 NormalL : NORMAL;
            };

            struct PS_INPUT
            {
                float4 PosH : SV_POSITION;
                float4 ColorH : COLOR;
                float3 NormalW : NORMAL;
            };

            PS_INPUT VS(VS_INPUT input) {
                PS_INPUT output;

                output.PosH = mul(float4(input.PosL, 1.0f), gWVP);
                output.ColorH = input.ColorL;
                output.NormalW = input.NormalL;

                return output;
            }

            float4 PS(PS_INPUT input) : SV_TARGET {
                // Ambient light
                float3 ambientColor = float3(0.2f, 0.2f, 0.2f); // Fixed ambient light itensity

                // Luz direcional
                float3 lightDir = normalize(float3(-1.0f, 1.0f, 1.0f)); // Light direction (from the surface to the light source)
                // float3 lightDir = normalize(float3(1.0f, -1.0f, -1.0f)); // Light direction (from the surface to the light source)
                float3 lightColor = float3(1.0f, 1.0f, 1.0f); // White light
                float diff = max(dot(input.NormalW, lightDir), 0.0f); // Diffuse intensity calculation
                float3 diffuse = diff * lightColor; // Multiply the diffuse intensity by the light color

                // Add the ambient and diffuse light
                float3 finalColor = (ambientColor + diffuse) * input.ColorH.rgb;
                return float4(finalColor, input.ColorH.a);
            }
        )";

        const char* PosColorNormalWithLightRotation = R"(
            cbuffer CB : register(b0)
            {
                float4x4 gWVP;
                float4x4 gW;
                float4x4 gV;
                float4x4 gP;
            };

            cbuffer LightBuffer : register(b1)
            {
                float4 diffuseColor;
                float3 lightDirection;
                float padding;
            };

            struct VS_INPUT
            {
                float3 PosL : POSITION;
                float4 ColorL : COLOR;
                float3 NormalL : NORMAL;
            };

            struct PS_INPUT
            {
                float4 PosH : SV_POSITION;
                float4 ColorH : COLOR;
                float3 NormalW : NORMAL;
            };

            PS_INPUT VS(VS_INPUT input) {
                PS_INPUT output;

                // Calculate the position of the vertex against the world, view, and projection matrices.
                output.PosH = mul(float4(input.PosL, 1.0f), gW);
                output.PosH = mul(output.PosH, gV);
                output.PosH = mul(output.PosH, gP);

                // Calculate the normal vector against the world matrix only.
                output.NormalW = mul(input.NormalL, (float3x3)gW);
                
                // Normalize the normal vector.
                output.NormalW = normalize(output.NormalW);

                output.ColorH = input.ColorL;

                return output;
            }

            float4 PS(PS_INPUT input) : SV_TARGET {
                // Negate the light direction
                float3 lightDir = -lightDirection;

                // Diffuse light color
                float3 diffuseColor = float3(1.0f, 0.3f, 1.0f);

                // Calculate the intensity of the diffuse light
                float diffIntensity = saturate(dot(input.NormalW, lightDir));
                float3 diffuse = diffuseColor.rgb * diffIntensity;

                // Ambient light
                float3 ambient = float3(1.0f, 0.3f, 1.0f);

                // Combine the two light sources (ambient and diffuse)
                float3 finalColor = ambient + diffuse;

                // Multiply the final color by the material color
                return float4(input.ColorH.rgb * finalColor, input.ColorH.a);
            }
        )";
	}
}

#endif // JOJ_PLATFORM_WINDOWS