#ifndef _JOJ_D3D11_SHADER_H
#define _JOJ_D3D11_SHADER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "core/defines.h"

#if JPLATFORM_WINDOWS

#include "renderer/shader.h"
#include <d3d11.h>

namespace joj
{
    struct VertexShader
    {
        ID3D11VertexShader* vertex_shader;        // Manages Vertex Shader Program and control Vertex Shader Stage
        ID3DBlob* vsblob;
    };

    struct PixelShader
    {
        ID3D11PixelShader* pixel_shader;          // Manages Pixel Shader Program and controls Pixel Shader Stage
        ID3DBlob* psblob;
    };

    struct GeometryShader
    {
        ID3D11GeometryShader* geometry_shader;    // Manages Geometry Shader Program and controls Geometry Shader Stage
        ID3DBlob* gsblob;
    };

    struct ComputeShader
    {
        ID3D11ComputeShader* compute_shader;      // Manages Compute Shader Program and controls Compute Shader Stage
        ID3DBlob* csblob;
    };

    class JAPI D3D11Shader : public Shader
    {
    public:
        D3D11Shader();
        ~D3D11Shader();

        // Compile from memory
        void compile_vertex_shader(const std::string& vertex_shader,
            const std::string& entry_point, const ShaderModel shader_model) override;

        // Compile from memory
        void compile_pixel_shader(const std::string& pixel_shader,
            const std::string& entry_point, const ShaderModel shader_model) override;

        void compile_vertex_shader_from_file(const std::string& vertex_path,
            const std::string& entry_point, const ShaderModel shader_model) override;

        void compile_pixel_shader_from_file(const std::string& pixel_path,
            const std::string& entry_point, const ShaderModel shader_model) override;

        void compile_geometry_shader_from_file(const std::string& geometry_path,
            const std::string& entry_point, const ShaderModel shader_model) override;

        void compile_compute_shader_from_file(const std::string& compute_path,
            const std::string& entry_point, const ShaderModel shader_model) override;

        ErrorCode create_vertex_shader(const GraphicsDevice& device) override;
        ErrorCode create_pixel_shader(const GraphicsDevice& device) override;

        void bind_vertex_shader(CommandList& cmd_list) override;
        void bind_pixel_shader(CommandList& cmd_list) override;

        VertexShader& get_vertex_shader() override;
        PixelShader& get_pixel_shader() override;
        GeometryShader& get_geometry_shader() override;
        ComputeShader& get_compute_shader() override;

    private:
        VertexShader m_vertex_shader;
        PixelShader m_pixel_shader;
        GeometryShader m_geometry_shader;
        ComputeShader m_compute_shader;
    };
}

#endif // JPLATFORM_WINDOWS

#endif // _JOJ_D3D11_SHADER_H