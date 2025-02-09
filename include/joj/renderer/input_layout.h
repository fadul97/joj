#ifndef _JOJ_INPUT_LAYOUT_H
#define _JOJ_INPUT_LAYOUT_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "core/defines.h"

#include "core/error_code.h"
#include <string>
#include "data_format.h"
#include "renderer.h"
#include <vector>
#include "shader.h"

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

    struct InputLayoutData;

    class JAPI InputLayout
    {
    public:
        InputLayout();
        virtual ~InputLayout();

        void add(InputDesc& desc);

        virtual ErrorCode create(const GraphicsDevice& device, VertexShader& shader) = 0;

        virtual void bind(CommandList& cmd_list) = 0;

        virtual InputLayoutData& get_data() = 0;

        std::vector<InputDesc>& get_input_layout();

    protected:
        std::vector<InputDesc> m_input_desc;
    };

    inline void InputLayout::add(InputDesc& desc)
    { m_input_desc.push_back(desc); }

    inline std::vector<InputDesc>& InputLayout::get_input_layout()
    { return m_input_desc; }
}

#endif // _JOJ_INPUT_LAYOUT_H