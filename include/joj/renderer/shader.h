#ifndef _JOJ_SHADER_H
#define _JOJ_SHADER_H

#include "joj/core/defines.h"

#include "joj/core/error_code.h"
#include "shader_model.h"
#include <string>

namespace joj
{
    /**
     * @brief GraphicsDevice is a struct that holds the graphics device.
     * It is API dependent, so each API will have its own implementation.
     *  
     * Forward declaration to avoid increasing compile time.
     */
    struct GraphicsDevice;

    /**
     * @brief CommandList is a struct that holds the command list.
     * It is API dependent, so each API will have its own implementation.
     * 
     * Forward declaration to avoid increasing compile time.
     * 
     */
    struct CommandList;

    /**
     * @brief VertexShader is a struct that holds the vertex shader data.
     * It is API dependent, so each API will have its own implementation.
     * 
     * Forward declaration to avoid increasing compile time.
     */
    struct VertexShader;

    /**
     * @brief PixelShader is a struct that holds the pixel shader data.
     * It is API dependent, so each API will have its own implementation.
     * 
     * Forward declaration to avoid increasing compile time.
     */
    struct PixelShader;

    /**
     * @brief InputLayoutData is a struct that holds the input layout data.
     * It is API dependent, so each API will have its own implementation.
     * 
     * Forward declaration to avoid increasing compile time.
     */
    struct InputLayoutData;

    /**
     * @brief IShader is an interface that represents a shader.
     * It is API dependent, so each API will have its own implementation.
     * 
     * It may or may not hold a pointer to the graphics device and the command list.
     * It is up to the API to decide whether it is necessary or not.
     */
    class JOJ_API IShader
    {
    public:
        /**
         * @brief Construct a new IShader object.
         */
        IShader();

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
        IShader(GraphicsDevice* device, CommandList* cmd_list);

        /**
         * @brief Destroy the IShader object.
         */
        virtual ~IShader();

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
         virtual ErrorCode compile_vertex_shader(const std::string& vertex_shader,
            const char* entry_point, const ShaderModel shader_model) = 0;

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
        virtual ErrorCode compile_pixel_shader(const std::string& pixel_shader,
            const char* entry_point, const ShaderModel shader_model) = 0;

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
        virtual ErrorCode compile_vertex_shader_from_file(const std::string& vertex_path,
            const char* entry_point, const ShaderModel shader_model) = 0;
        
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
        virtual ErrorCode compile_pixel_shader_from_file(const std::string& pixel_path,
            const char* entry_point, const ShaderModel shader_model) = 0;

        /**
         * @brief Bind the shader to the pipeline.
         */
        virtual void bind() = 0;

        /**
         * @brief Unbind the shader from the pipeline.
         */
        virtual void unbind() = 0;

        /**
         * @brief Destroy the shader.
         * It will release the resources that were created in the compile methods.
         * 
         * @note This method is called in the destructor of the IShader class.
         * If the user wants to reuse the shader, it is recommended to call this method separately
         * before compiling the shader again.
         */
        virtual void destroy() = 0;

        /**
         * @brief Get the vertex shader object data.
         * 
         * @return VertexShader* The vertex shader object data.
         */
        virtual VertexShader* get_vertex_shader() = 0;

        /**
         * @brief Get the pixel shader object data.
         * 
         * @return PixelShader* The pixel shader object data.
         */
        virtual PixelShader* get_pixel_shader() = 0;

        /**
         * @brief Get the input layout object data of the Vertex Shader.
         * 
         * @return InputLayoutData* The input layout object data.
         */
        virtual InputLayoutData* get_input_layout() = 0;
    };
}

#endif // _JOJ_SHADER_H