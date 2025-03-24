#ifndef _JOJ_D3D11_CONSTANT_BUFFER_H
#define _JOJ_D3D11_CONSTANT_BUFFER_H

#include "joj/core/defines.h"

#if JOJ_PLATFORM_WINDOWS

#include "joj/renderer/constant_buffer.h"
#include "constant_buffer_data_d3d11.h"

namespace joj
{
    /**
     * @brief Calculate the size of the Constant Buffer in bytes.
     * This is needed because the size of the buffer must be a multiple of the minimum hardware
     * allocation size (usually 256 bytes).
     * 
     * @param byte_size The size of the buffer in bytes.
     * @return u32 The size of the Constant Buffer in bytes.
     */
    u32 calculate_cb_byte_size(const u32 byte_size);

    /**
     * @brief D3D11ConstantBuffer is a class that represents a Constant Buffer.
     * It is API dependent, so each API will have its own implementation.
     * 
     * It holds a pointer to the graphics device and the command list.
     * It does not manage the memory of the device and the command list.
     */
    class JOJ_API D3D11ConstantBuffer : public IConstantBuffer
    {
    public:
        /**
         * @brief Construct a new D3D11ConstantBuffer object.
         */
        D3D11ConstantBuffer();

        /**
         * @brief Construct a new D3D11ConstantBuffer object.
         * 
         * @param device The graphics device.
         * @param cmd_list The command list.
         * 
         * @note The device and the command list are optional.
         * It is up to the API to decide whether it is necessary or not.
         * 
         * @note The memory of the device and the command list will not be managed by the D3D11ConstantBuffer class.
         */
        D3D11ConstantBuffer(GraphicsDevice* device, CommandList* cmd_list);

        /**
         * @brief Destroy the D3D11ConstantBuffer object.
         */
        ~D3D11ConstantBuffer() override;

        /**
         * @brief Create the Constant Buffer.
         * 
         * @param usage Type of usage for the Constant Buffer.
         * @param cpu_access_flags Access type that the CPU has on the Constant Buffer.
         * @param byte_width The size of the buffer in bytes.
         * @param data The data that will be copied to the buffer.
         * @return ErrorCode The error code that will be returned if the buffer
         * creation fails. If the buffer creation is successful, it will return
         * ErrorCode::OK.
         */
        ErrorCode create(const BufferUsage usage, const CPUAccessType cpu_access_flags,
            const u32 byte_width, const void* data) override;

        /**
         * @brief Bind the Constant Buffer to the Vertex Shader in the pipeline.
         * 
         * @param start_slot The first input slot for binding.
         * @param num_buffers The number of vertex buffers to bind.
         */
        void bind_to_vertex_shader(const u32 start_slot, const u32 num_buffers) override;

        /**
         * @brief Bind the Constant Buffer to the Pixel Shader in the pipeline.
         * 
         * @param start_slot The first input slot for binding.
         * @param num_buffers The number of pixel buffers to bind.
         */
        void bind_to_pixel_shader(const u32 start_slot, const u32 num_buffers) override;

        /**
         * @brief Unbind the Constant Buffer from Vertex Shader in the pipeline.
         */
        void unbind_from_vertex_shader() override;

        /**
         * @brief Unbind the Constant Buffer from Pixel Shader in the pipeline.
         */
        void unbind_from_pixel_shader() override;

        /**
         * @brief Return if the Constant Buffer is filled.
         * 
         * @return b8 True if the Constant Buffer is filled, false otherwise.
         */
        b8 is_filled() const;

        /**
         * @brief Cleanup the Constant Buffer.
         * It will release the resources that were created in the create method.
         * 
         * This method is called in the destructor of the D3D11ConstantBuffer class.
         */
        void cleanup() override;

    protected:
        /** @brief The Constant Buffer data. */
        ConstantBufferData m_data;

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
         * @brief Update the Constant Buffer.
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

#endif // _JOJ_D3D11_CONSTANT_BUFFER_H