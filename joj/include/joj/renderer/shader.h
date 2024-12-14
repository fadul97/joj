#ifndef _JOJ_SHADER_H
#define _JOJ_SHADER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "error_code.h"
#include <string>
#include "renderer.h"

namespace joj
{
    enum class ShaderModel
    {
        Default
    };

    struct VertexShader;

    struct PixelShader;

    struct GeometryShader;

    struct ComputeShader;

    class JAPI Shader
    {
    public:
        Shader();
        virtual ~Shader();

        // Compile from file
        virtual void compile_vertex_shader(const std::string& vertex_path,
            const std::string& entry_point, const ShaderModel shader_model) = 0;
        
        // Compile from file
        virtual void compile_pixel_shader(const std::string& pixel_path,
            const std::string& entry_point, const ShaderModel shader_model) = 0;
        
        // Compile from file
        virtual void compile_geometry_shader(const std::string& pixel_path,
            const std::string& entry_point, const ShaderModel shader_model) = 0;

        // Compile from file
        virtual void compile_compute_shader(const std::string& pixel_path,
            const std::string& entry_point, const ShaderModel shader_model) = 0;

        virtual ErrorCode create_vertex_shader(GraphicsDevice& device) = 0;
        virtual ErrorCode create_pixel_shader(GraphicsDevice& device) = 0;

        virtual void bind_vertex_shader(CommandList& cmd_list) = 0;
        virtual void bind_pixel_shader(CommandList& cmd_list) = 0;

        virtual VertexShader& get_vertex_shader() = 0;
        virtual PixelShader& get_pixel_shader() = 0;
        virtual GeometryShader& get_geometry_shader() = 0;
        virtual ComputeShader& get_compute_shader() = 0;
    };
}

#endif // _JOJ_SHADER_H