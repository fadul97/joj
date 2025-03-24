#ifndef _JOJ_SHADER_LIBRARY_H
#define _JOJ_SHADER_LIBRARY_H

#include "joj/core/defines.h"

namespace joj
{
	namespace ShaderLibrary
	{
                /**
                 * @brief A basic vertex shader that only passes the position and color.
                 * 
                 */
                JOJ_EXTERN const char* VertexShaderSimple;

                /**
                 * @brief A basic pixel shader that only returns the color.
                 * 
                 */
                JOJ_EXTERN const char* PixelShaderSimple;

                /**
                 * @brief A basic vertex shader that only passes the position and color.
                 * It has a simple camera that transforms the vertices, the constant buffer
                 * only holds a float4x4 matrix.
                 */
                JOJ_EXTERN const char* VertexShaderSimpleCamera;
	}
}

#endif // _JOJ_SHADER_LIBRARY_H