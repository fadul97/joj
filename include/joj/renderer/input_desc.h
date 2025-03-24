#ifndef _JOJ_INPUT_DESC_H
#define _JOJ_INPUT_DESC_H

#include "joj/core/defines.h"

#include <string>
#include "data_format.h"

namespace joj
{
    /**
     * @brief InputClassification is an enum class that holds the type of classification
     * the shader will use for the input layout.
     * 
     * It is used to set the input classification of the input layout.
     */
    enum class InputClassification
    {
        Undefined,
        PerVertexData,
        PerInstanceData
    };

    /**
     * @brief InputDesc is a struct that holds the input description.
     * It is used to set the input layout of the Shader in the pipeline.
     */
    struct JOJ_API InputDesc
    {
        /** @brief The semantic name of the input layout. */
        std::string semantic_name;

        /** @brief The semantic index of the input layout. */
        u32 semantic_index{ 0 };

        /** @brief The data format of the input layout. */
        DataFormat format{ DataFormat::UNDEFINED };

        /** @brief The input slot of the input layout. */
        u32 input_slot{ 0 };

        /** @brief The aligned byte offset of the input layout. */
        u32 aligned_byte_offset{ 0 };

        /** @brief The input classification of the input layout. */
        InputClassification classification{ InputClassification::Undefined };

        /** @brief The instance data step rate of the input layout. */
        u32 instance_data_step_rate{ 0 };
    };
}

#endif // _JOJ_INPUT_DESC_H