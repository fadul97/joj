#ifndef _JOJ_INDEX_BUFFER_H
#define _JOJ_INDEX_BUFFER_H

#include "core/defines.h"

#include "data_format.h"
#include "core/error_code.h"
#include "renderer.h"

namespace joj
{
    struct IBData;

    class JAPI IndexBuffer
    {
    public:
        IndexBuffer();
        virtual ~IndexBuffer();

        virtual void setup(u32 byte_width, const void* data) = 0;

        virtual ErrorCode create(const GraphicsDevice& device) = 0;

        virtual void bind(CommandList& cmd_list, DataFormat format,
            const u32 offset) = 0;

        virtual IBData& get_data() = 0;
    };
}

#endif // _JOJ_INDEX_BUFFER_H