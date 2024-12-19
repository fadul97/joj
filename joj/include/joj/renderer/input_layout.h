#ifndef _JOJ_INPUT_LAYOUT_H
#define _JOJ_INPUT_LAYOUT_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include <string>
#include "data_format.h"
#include "renderer.h"

namespace joj
{
    enum class InputClassification
    {
        PerVertexData,
        PerInstanceData
    };

    struct InputDesc
    {
        std::string semantic_name;
        u32 semantic_index;
        DataFormat format;
        u32 input_slot;
        u32 aligned_byte_offset;
        InputClassification classification;
        u32 instance_data_step_rate;
    };

    class JAPI InputLayout
    {
    public:
        InputLayout();
        virtual ~InputLayout();

        virtual ErrorCode create(GraphicsDevice& device, InputDesc& desc) = 0;

        virtual void bind(CommandList& cmd_list) = 0;

        virtual InputDesc& get_data() = 0;
    };
}

#endif // _JOJ_INPUT_LAYOUT_H