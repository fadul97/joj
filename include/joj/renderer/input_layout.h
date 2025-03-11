#ifndef _JOJ_INPUT_LAYOUT_H
#define _JOJ_INPUT_LAYOUT_H

#include "core/defines.h"

#include "core/error_code.h"
#include <string>
#include "data_format.h"
#include "renderer.h"
#include <vector>
#include "shader.h"

namespace joj
{
    enum class JOJ_API InputClassification
    {
        Undefined,
        PerVertexData,
        PerInstanceData
    };

    struct JOJ_API InputDesc
    {
        std::string semantic_name;
        u32 semantic_index{ 0 };
        DataFormat format{ DataFormat::UNDEFINED };
        u32 input_slot{ 0 };
        u32 aligned_byte_offset{ 0 };
        InputClassification classification{ InputClassification::Undefined };
        u32 instance_data_step_rate{ 0 };
    };

    struct InputLayoutData;

    using JVectorInputDesc = std::vector<InputDesc>;

    class JOJ_API InputLayout
    {
    public:
        InputLayout();
        virtual ~InputLayout();

        void add(InputDesc& desc);

        virtual ErrorCode create(const GraphicsDevice& device, VertexShader& shader) = 0;

        virtual void bind(CommandList& cmd_list) = 0;

        virtual InputLayoutData& get_data() = 0;

        JVectorInputDesc& get_input_layout();

    protected:
        JVectorInputDesc m_input_desc;
    };

    inline void InputLayout::add(InputDesc& desc)
    { m_input_desc.push_back(desc); }

    inline JVectorInputDesc& InputLayout::get_input_layout()
    { return m_input_desc; }
}

#endif // _JOJ_INPUT_LAYOUT_H