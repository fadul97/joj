#ifndef _JOJ_D3D11_INDEX_BUFFER_H
#define _JOJ_D3D11_INDEX_BUFFER_H

#include "joj/core/defines.h"

#if JOJ_PLATFORM_WINDOWS

#include "joj/renderer/index_buffer.h"
#include "index_buffer_data_d3d11.h"

namespace joj
{
    /**
     * @brief D3D11IndexBuffer is a class that represents a Index Buffer.
     * It is API dependent, so each API will have its own implementation.
     * 
     * It holds a pointer to the graphics device and the command list.
     * It does not manage the memory of the device and the command list.
     */
    class JOJ_API D3D11IndexBuffer : public IIndexBuffer
    {
    public:
        /**
         * @brief Construct a new D3D11IndexBuffer object.
         */
        D3D11IndexBuffer();

        /**
         * @brief Construct a new D3D11IndexBuffer object.
         * 
         * @param device The graphics device.
         * @param cmd_list The command list.
         * 
         * @note The device and the command list are optional.
         * It is up to the API to decide whether it is necessary or not.
         * 
         * @note The memory of the device and the command list will not be managed by the D3D11IndexBuffer class.
         */
        D3D11IndexBuffer(GraphicsDevice* device, CommandList* cmd_list);

        /**
         * @brief Destroy the D3D11IndexBuffer object.
         */
        ~D3D11IndexBuffer() override;

        /**
         * @brief Create the Index Buffer.
         * 
         * @param usage Type of usage for the Index Buffer.
         * @param cpu_access_flags Access type that the CPU has on the Index Buffer.
         * @param byte_width The size of the buffer in bytes.
         * @param data The data that will be copied to the buffer.
         * @return ErrorCode The error code that will be returned if the buffer
         * creation fails. If the buffer creation is successful, it will return
         * ErrorCode::OK.
         */
        ErrorCode create(const BufferUsage usage, const CPUAccessType cpu_access_flags,
            const u32 byte_width, const void* data) override;

        /**
         * @brief Bind the Index Buffer to the pipeline.
         * 
         * @param format The format of the data.
         * @param offset The offset of the data.
         */
        void bind(const DataFormat format, const u32 offset) override;

        /**
         * @brief Unbind the Index Buffer from the pipeline.
         */
        void unbind() override;

        /**
         * @brief Return if the Index Buffer is filled.
         * 
         * @return b8 True if the Index Buffer is filled, false otherwise.
         */
        b8 is_filled() const;

        /**
         * @brief Cleanup the Index Buffer.
         * It will release the resources that were created in the create method.
         * 
         * This method is called in the destructor of the D3D11IndexBuffer class.
         */
        void cleanup() override;

    protected:
        /** @brief The Index Buffer data. */
        IndexBufferData m_data;

        /** @brief The filled flag. */
        b8 m_filled;

        /**
         * @brief The graphics device.
         */
        GraphicsDevice* m_device;

        /**
         * @brief The command list.
         */
        CommandList* m_cmd_list;

        /**
         * @brief Update the Index Buffer.
         * 
         * @param data The data that will be copied to the buffer.
         * @param sizeof_data The size of the data in bytes.
         * @return ErrorCode The error code that will be returned if the buffer
         * update fails. If the buffer update is successful, it will return
         * ErrorCode::OK.
         */
        ErrorCode update_internal(const void* data, const u32 sizeof_data) override;
    };
}

#endif // JOJ_PLATFORM_WINDOWS

#endif // _JOJ_D3D11_INDEX_BUFFER_H