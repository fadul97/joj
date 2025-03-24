#ifndef _JOJ_D3D11_SHADER_H
#define _JOJ_D3D11_SHADER_H

#include "joj/core/defines.h"

#if JOJ_PLATFORM_WINDOWS

#include "joj/renderer/shader.h"
#include <d3d11.h>
#include "vertex_shader_data_d3d11.h"
#include "pixel_shader_data_d3d11.h"
#include "input_layout_data_d3d11.h"
#include <vector>
#include "joj/renderer/input_desc.h"

namespace joj
{
    /**
     * @brief D3D11Shader is an interface that represents a shader.
     * It is API dependent, so each API will have its own implementation.
     * 
     * It may or may not hold a pointer to the graphics device and the command list.
     * It is up to the API to decide whether it is necessary or not.
     */
    class JOJ_API D3D11Shader : public IShader
    {
    public:
        /**
         * @brief Construct a new D3D11Shader object.
         */
        D3D11Shader();

        /**
         * @brief Construct a new IShader object.
         * 
         * @param device The graphics device.
         * @param cmd_list The command list.
         * 
         * @note The device and the command list are optional.
         * It is up to the API to decide whether it is necessary or not.
         * 
         * @note The memory of the device and the command list will not be managed by the IShader class.
         */
        D3D11Shader(GraphicsDevice* device, CommandList* cmd_list);

        /**
         * @brief Destroy the D3D11Shader object.
         */
        ~D3D11Shader() override;

         /**
          * @brief Compile the vertex shader from char array.
          * 
          * @param vertex_shader The vertex shader.
          * @param entry_point The entry point of the shader.
          * @param shader_model The version of the shader.
          * 
          * @return ErrorCode The error code that will be returned if the shader
          * initialization fails. If the shader initialization is successful, it will return
          * ErrorCode::OK.
          */
        ErrorCode compile_vertex_shader(const std::string& vertex_shader,
            const char* entry_point, const ShaderModel shader_model) override;

        /**
         * @brief Compile the pixel shader from char array.
         * 
         * @param pixel_shader The pixel shader.
         * @param entry_point The entry point of the shader.
         * @param shader_model The version of the shader.
         * 
         * @return ErrorCode The error code that will be returned if the shader
         * initialization fails. If the shader initialization is successful, it will return
         * ErrorCode::OK.
         */
        ErrorCode compile_pixel_shader(const std::string& pixel_shader,
            const char* entry_point, const ShaderModel shader_model) override;

        /**
         * @brief Compile the vertex shader from file.
         * 
         * @param vertex_path The path to the vertex shader file.
         * @param entry_point The entry point of the shader.
         * @param shader_model The version of the shader.
         * 
         * @return ErrorCode The error code that will be returned if the shader
         * initialization fails. If the shader initialization is successful, it will return
         * ErrorCode::OK.
         */
        ErrorCode compile_vertex_shader_from_file(const std::string& vertex_path,
            const char* entry_point, const ShaderModel shader_model) override;
        
        /**
         * @brief Compile the pixel shader from file.
         * 
         * @param pixel_path The path to the pixel shader file.
         * @param entry_point The entry point of the shader.
         * @param shader_model The version of the shader.
         * 
         * @return ErrorCode The error code that will be returned if the shader
         * initialization fails. If the shader initialization is successful, it will return
         * ErrorCode::OK.
         */
        ErrorCode compile_pixel_shader_from_file(const std::string& pixel_path,
            const char* entry_point, const ShaderModel shader_model) override;

        /**
         * @brief Bind the shader to the pipeline.
         */
        void bind() override;

        /**
         * @brief Unbind the shader from the pipeline.
         */
        void unbind() override;

        /**
         * @brief Destroy the shader.
         * It will release the resources that were created in the compile methods.
         * 
         * @note This method is called in the destructor of the D3D11Shader class.
         * If the user wants to reuse the shader, it is recommended to call this method separately
         * before compiling the shader again.
         */
        void destroy() override;

        /**
         * @brief Create the input layout after compiling the vertex shader.
         * 
         * @return ErrorCode The error code that will be returned if the input layout
         * creation fails. If the input layout creation is successful, it will return
         * ErrorCode::OK.
         */
        ErrorCode create_input_layout(std::vector<InputDesc>& input_desc);

        /**
         * @brief Get the vertex shader object data.
         * 
         * @return VertexShader* The vertex shader object data.
         */
        VertexShader* get_vertex_shader() override;

        /**
         * @brief Get the pixel shader object data.
         * 
         * @return PixelShader* The pixel shader object data.
         */
        PixelShader* get_pixel_shader() override;

        /**
         * @brief Get the input layout object data of the Vertex Shader.
         * 
         * @return InputLayoutData* The input layout object data.
         */
        InputLayoutData* get_input_layout() override;

    private:
        /**
         * @brief The vertex shader data.
         */
        VertexShader m_vertex_shader;

        /**
         * @brief The pixel shader data.
         */
        PixelShader m_pixel_shader;

        /**
         * @brief The input layout for the current Vertex Shader.
         */
        InputLayoutData m_input_layout;

        /**
         * @brief The graphics device.
         */
        GraphicsDevice* m_device;

        /**
         * @brief The command list.
         */
        CommandList* m_cmd_list;

        /**
         * @brief Create the vertex shader after compiling it.
         * 
         * @return ErrorCode The error code that will be returned if the vertex shader
         * creation fails. If the vertex shader creation is successful, it will return
         * ErrorCode::OK.
         */
        ErrorCode create_vertex_shader();

        /**
         * @brief Create the pixel shader after compiling it.
         * 
         * @return ErrorCode The error code that will be returned if the pixel shader
         * creation fails. If the pixel shader creation is successful, it will return
         * ErrorCode::OK.
         */
        ErrorCode create_pixel_shader();
    };
}

#endif // JOJ_PLATFORM_WINDOWS

#endif // _JOJ_D3D11_SHADER_H