#include "joj/renderer/shader_library.h"

#if JOJ_PLATFORM_WINDOWS

namespace joj
{
	namespace ShaderLibrary
	{
        /**
         * @brief A basic vertex shader that only passes the position and color.
         * 
         */
        const char* VertexShaderSimple = R"(
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
        )";

        /**
         * @brief A basic pixel shader that only returns the color.
         * 
         */
        const char* PixelShaderSimple = R"(
            struct PS_INPUT {
                float4 position : SV_POSITION;
                float4 color : COLOR;
            };

            float4 PS(PS_INPUT input) : SV_TARGET {
                return input.color;
            }
        )";

        /**
         * @brief A basic vertex shader that only passes the position and color.
         * It has a simple camera that transforms the vertices, the constant buffer
         * only holds a float4x4 matrix.
         */
        const char* VertexShaderSimpleCamera = R"(
            cbuffer CB : register(b0)
            {
                float4x4 gWVP;
            };

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

                output.PosH = mul(float4(input.PosL, 1.0f), gWVP);
                output.ColorH = input.ColorL;

                return output;
            }
        )";
	}
}

#endif // JOJ_PLATFORM_WINDOWS