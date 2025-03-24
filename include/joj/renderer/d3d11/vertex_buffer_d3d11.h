#ifndef _JOJ_D3D11_VERTEX_BUFFER_H
#define _JOJ_D3D11_VERTEX_BUFFER_H

#include "joj/core/defines.h"

#if JOJ_PLATFORM_WINDOWS

#include "joj/renderer/vertex_buffer.h"
#include "vertex_buffer_data_d3d11.h"

namespace joj
{
    /**
     * @brief D3D11VertexBuffer is a class that represents a Vertex Buffer.
     * It is API dependent, so each API will have its own implementation.
     * 
     * It holds a pointer to the graphics device and the command list.
     * It does not manage the memory of the device and the command list.
     */
    class JOJ_API D3D11VertexBuffer : public IVertexBuffer
    {
    public:
        /**
         * @brief Construct a new D3D11VertexBuffer object.
         */
        D3D11VertexBuffer();

        /**
         * @brief Construct a new D3D11VertexBuffer object.
         * 
         * @param device The graphics device.
         * @param cmd_list The command list.
         * 
         * @note The device and the command list are optional.
         * It is up to the API to decide whether it is necessary or not.
         * 
         * @note The memory of the device and the command list will not be managed by the D3D11VertexBuffer class.
         */
        D3D11VertexBuffer(GraphicsDevice* device, CommandList* cmd_list);

        /**
         * @brief Destroy the D3D11VertexBuffer object.
         */
        ~D3D11VertexBuffer() override;

        /**
         * @brief Create the Vertex Buffer.
         * 
         * @param usage Type of usage for the Vertex Buffer.
         * @param cpu_access_flags Access type that the CPU has on the Vertex Buffer.
         * @param byte_width The size of the buffer in bytes.
         * @param data The data that will be copied to the buffer.
         * @return ErrorCode The error code that will be returned if the buffer
         * creation fails. If the buffer creation is successful, it will return
         * ErrorCode::OK.
         */
        ErrorCode create(const BufferUsage usage, const CPUAccessType cpu_access_flags,
            const u32 byte_width, const void* data) override;

        /**
         * @brief Bind the Vertex Buffer to the pipeline.
         * 
         * @param start_slot The first input slot for binding.
         * @param num_buffers The number of vertex buffers to bind.
         * @param stride The stride of the vertex buffer.
         * @param offset The offset of the vertex buffer.
         */
        void bind(const u32 start_slot, const u32 num_buffers,
            const u32* stride, const u32* offset) override;

        /**
         * @brief Unbind the Vertex Buffer from the pipeline.
         */
        void unbind() override;

        /**
         * @brief Return if the Vertex Buffer is filled.
         * 
         * @return b8 True if the Vertex Buffer is filled, false otherwise.
         */
        b8 is_filled() const;

        /**
         * @brief Cleanup the Vertex Buffer.
         * It will release the resources that were created in the create method.
         * 
         * This method is called in the destructor of the D3D11VertexBuffer class.
         */
        void cleanup() override;

    protected:
        /** @brief The Vertex Buffer data. */
        VertexBufferData m_data;

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
         * @brief Update the Vertex Buffer.
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

#endif // _JOJ_D3D11_VERTEX_BUFFER_H