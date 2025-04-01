#ifndef _JOJ_SHADER_LIBRARY_H
#define _JOJ_SHADER_LIBRARY_H

#include "joj/core/defines.h"

namespace joj
{
	namespace ShaderLibrary
	{
        /**
         * @brief A basic vertex shader that only passes the position and color
         * and only returns the color.
         */
        JOJ_EXTERN const char* SimpleColor;

        /**
         * @brief A basic vertex shader that only passes the position, normal and color
         * and only returns the color.
         */
        JOJ_EXTERN const char* PosColorNormalSimple;

        /**
         * @brief A basic vertex shader that passes the position, color and normal.
         * It has a simple camera that transforms the vertices, the constant buffer
         * only holds a float4x4 matrix.
         */
        JOJ_EXTERN const char* PosColorNormalAndCamera;

        /**
         * @brief A basic vertex shader that passes the position, color, normal.
         * It has a simple camera that transforms the vertices, the constant buffer
         * only holds a float4x4 matrix.
         */
        JOJ_EXTERN const char* LightWithPosColorNormalAndCamera;

        JOJ_EXTERN const char* PosColorNormalWithLightRotation;
	}
}

#endif // _JOJ_SHADER_LIBRARY_H