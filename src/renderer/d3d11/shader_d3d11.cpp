#include "joj/renderer/d3d11/shader_d3d11.h"

#if JOJ_PLATFORM_WINDOWS

#include <d3dcompiler.h>
#include "joj/core/logger.h"
#include "joj/renderer/d3d11/graphics_device_d3d11.h"
#include "joj/renderer/d3d11/command_list_d3d11.h"
#include "joj/core/jmacros.h"

joj::D3D11Shader::D3D11Shader()
    : m_device(nullptr), m_cmd_list(nullptr)
{
    m_vertex_shader.vertex_shader = nullptr;
    m_vertex_shader.vsblob = nullptr;

    m_pixel_shader.pixel_shader = nullptr;
    m_pixel_shader.psblob = nullptr;

    m_input_layout.input_layout = nullptr;
}

joj::D3D11Shader::D3D11Shader(GraphicsDevice* device, CommandList* cmd_list)
    : m_device(device), m_cmd_list(cmd_list)
{
    m_vertex_shader.vertex_shader = nullptr;
    m_vertex_shader.vsblob = nullptr;

    m_pixel_shader.pixel_shader = nullptr;
    m_pixel_shader.psblob = nullptr;

    m_input_layout.input_layout = nullptr;
}

joj::D3D11Shader::~D3D11Shader()
{
    destroy();
}

joj::ErrorCode joj::D3D11Shader::compile_vertex_shader(const std::string& vertex_shader,
    const char* entry_point, const ShaderModel shader_model)
{
    DWORD shader_flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef JOJ_DEBUG_MODE
	// Let compiler insert debug information into the output code
	shader_flags |= D3DCOMPILE_DEBUG;

	// Disable optimizations
    // Compiler will not validate the generated code -> Recommended to use only with successfully compiled shaders
	shader_flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // JOJ_DEBUG_MODE

    ID3DBlob* error_blob = nullptr;
    std::string model;
	switch (shader_model)
	{
	case ShaderModel::Default:
		model = "vs_5_0";
		break;
	default:
		model = "vs_5_0";
		break;
	}

    if (D3DCompile(
		vertex_shader.c_str(), vertex_shader.length(), nullptr, nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, entry_point, model.c_str(),
		shader_flags, 0, &m_vertex_shader.vsblob, &error_blob) != S_OK)
	{
		if (error_blob)
		{
			OutputDebugStringA((char*)error_blob->GetBufferPointer());
			JOJ_ERROR(ErrorCode::ERR_SHADER_VERTEX_COMPILATION, "%s",
				(char*)error_blob->GetBufferPointer());
			error_blob->Release();
            error_blob = nullptr;
		}
		else
		{
			JOJ_ERROR(ErrorCode::ERR_SHADER_VERTEX_COMPILATION,
				"Failed to compile Vertex Shader."          \
                "Unable to get information about the error.");
		}

        return ErrorCode::ERR_SHADER_VERTEX_COMPILATION;
	}

	if (error_blob)
        error_blob->Release();

    return create_vertex_shader();
}

joj::ErrorCode joj::D3D11Shader::compile_pixel_shader(const std::string& pixel_shader,
    const char* entry_point, const ShaderModel shader_model)
{
    DWORD shader_flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef JOJ_DEBUG_MODE
	// Let compiler insert debug information into the output code
	shader_flags |= D3DCOMPILE_DEBUG;

	// Disable optimizations
	shader_flags |= D3DCOMPILE_SKIP_OPTIMIZATION;      // Compiler will not validate the generated code -> Recommended to use only with successfully compiled shaders
#endif // JOJ_DEBUG_MODE

	ID3DBlob* error_blob = nullptr;

	std::string model;
	switch (shader_model)
	{
	case ShaderModel::Default:
		model = "ps_5_0";
		break;
	default:
		model = "ps_5_0";
		break;
	}

    if (D3DCompile(
		pixel_shader.c_str(), pixel_shader.length(), nullptr, nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, entry_point, model.c_str(),
		shader_flags, 0, &m_pixel_shader.psblob, &error_blob) != S_OK)
	{
		if (error_blob)
		{
			OutputDebugStringA((char*)error_blob->GetBufferPointer());
			JOJ_ERROR(ErrorCode::ERR_SHADER_PIXEL_COMPILATION, "%s",
				(char*)error_blob->GetBufferPointer());
			error_blob->Release();
            error_blob = nullptr;
		}
		else
		{
			JOJ_ERROR(ErrorCode::ERR_SHADER_PIXEL_COMPILATION,
				"Failed to compile Pixel Shader."           \
                "Unable to get information about the error.");
		}

        return ErrorCode::ERR_SHADER_PIXEL_COMPILATION;
	}

	if (error_blob)
		error_blob->Release();

    return create_pixel_shader();
}

joj::ErrorCode joj::D3D11Shader::compile_vertex_shader_from_file(const std::string& vertex_path,
    const char* entry_point, const ShaderModel shader_model)
{
    DWORD shader_flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef JOJ_DEBUG_MODE
	// Let compiler insert debug information into the output code
	shader_flags |= D3DCOMPILE_DEBUG;

	// Disable optimizations
	shader_flags |= D3DCOMPILE_SKIP_OPTIMIZATION;      // Compiler will not validate the generated code -> Recommended to use only with successfully compiled shaders
#endif // JOJ_DEBUG_MODE

	// --------------------------------
	// Vertex Shader
	// --------------------------------

	ID3DBlob* error_blob = nullptr;    // To get info about compilation

	std::string model;
	switch (shader_model)
	{
	case ShaderModel::Default:
		model = "vs_5_0";
		break;
	default:
		model = "vs_5_0";
		break;
	}

    std::wstring wide_path(vertex_path.begin(), vertex_path.end());

	// Compile Vertex Shader
	if (D3DCompileFromFile(wide_path.c_str(), nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, entry_point,
		model.c_str(), shader_flags, NULL, &m_vertex_shader.vsblob,
		&error_blob) != S_OK)
	{
		if (error_blob != nullptr)
		{
			OutputDebugStringA((char*)error_blob->GetBufferPointer());
			JOJ_ERROR(ErrorCode::ERR_SHADER_VERTEX_COMPILATION, "%s",
				(char*)error_blob->GetBufferPointer());
            error_blob->Release();
            error_blob = nullptr;
		}
		else
		{
			JOJ_ERROR(ErrorCode::ERR_SHADER_VERTEX_COMPILATION,
				"Failed to compile Vertex Shader."          \
                "Unable to get information about the error.");
		}

        return ErrorCode::ERR_SHADER_VERTEX_COMPILATION;
	}

	if (error_blob)
		error_blob->Release();

    return create_vertex_shader();
}

joj::ErrorCode joj::D3D11Shader::compile_pixel_shader_from_file(const std::string& pixel_path,
    const char* entry_point, const ShaderModel shader_model)
{
    DWORD shader_flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef JOJ_DEBUG_MODE
	// Let compiler insert debug information into the output code
	shader_flags |= D3DCOMPILE_DEBUG;

	// Disable optimizations
	shader_flags |= D3DCOMPILE_SKIP_OPTIMIZATION;      // Compiler will not validate the generated code -> Recommended to use only with successfully compiled shaders
#endif // JOJ_DEBUG_MODE

	// --------------------------------
	// Pixel Shader
	// --------------------------------

	ID3DBlob* error_blob = nullptr;    // To get info about compilation

	std::string model;
	switch (shader_model)
	{
	case ShaderModel::Default:
		model = "ps_5_0";
		break;
	default:
		model = "ps_5_0";
		break;
	}

	std::wstring wide_path(pixel_path.begin(), pixel_path.end());

	// Compile Pixel Shader
	if (D3DCompileFromFile(wide_path.c_str(), nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, entry_point,
		model.c_str(), shader_flags, NULL, &m_pixel_shader.psblob,
		&error_blob) != S_OK)
	{
		if (error_blob != nullptr)
		{
			OutputDebugStringA((char*)error_blob->GetBufferPointer());
			JOJ_ERROR(ErrorCode::ERR_SHADER_PIXEL_COMPILATION, "%s",
				(char*)error_blob->GetBufferPointer());
            error_blob->Release();
            error_blob = nullptr;
		}
		else
		{
			JOJ_ERROR(ErrorCode::ERR_SHADER_PIXEL_COMPILATION,
				"Failed to compile Pixel Shader."           \
                "Unable to get information about the error.");
		}

        return ErrorCode::ERR_SHADER_PIXEL_COMPILATION;
	}

	if (error_blob)
		error_blob->Release();

    return create_pixel_shader();
}

void joj::D3D11Shader::bind()
{
	JOJ_ASSERT(m_cmd_list, "CommandList is nullptr.");
	JOJ_ASSERT(m_vertex_shader.vertex_shader, "Vertex Shader is nullptr.");
	JOJ_ASSERT(m_pixel_shader.pixel_shader, "Pixel Shader is nullptr.");
	JOJ_ASSERT(m_input_layout.input_layout, "Input Layout is nullptr.");

    m_cmd_list->device_context->VSSetShader(m_vertex_shader.vertex_shader, nullptr, 0);
    m_cmd_list->device_context->PSSetShader(m_pixel_shader.pixel_shader, nullptr, 0);
    m_cmd_list->device_context->IASetInputLayout(m_input_layout.input_layout);
}

void joj::D3D11Shader::unbind()
{
	JOJ_ASSERT(m_cmd_list, "CommandList is nullptr.");

    m_cmd_list->device_context->VSSetShader(nullptr, nullptr, 0);
    m_cmd_list->device_context->PSSetShader(nullptr, nullptr, 0);
    m_cmd_list->device_context->IASetInputLayout(nullptr);
}

void joj::D3D11Shader::destroy()
{
	// Release Input Layout
	if (m_input_layout.input_layout)
	{
		m_input_layout.input_layout->Release();
		m_input_layout.input_layout = nullptr;
	}

    // Relase pixel shader bytecode
	if (m_pixel_shader.psblob)
	{
		m_pixel_shader.psblob->Release();
		m_pixel_shader.psblob = nullptr;
	}
    
    // Relase vertex shader bytecode
	if (m_vertex_shader.vsblob)
	{
		m_vertex_shader.vsblob->Release();
		m_vertex_shader.vsblob = nullptr;
	}

	// Relase Pixel Shader
	if (m_pixel_shader.pixel_shader)
	{
		m_pixel_shader.pixel_shader->Release();
		m_pixel_shader.pixel_shader = nullptr;
	}

	// Relase Vertex Shader
	if (m_vertex_shader.vertex_shader)
	{
		m_vertex_shader.vertex_shader->Release();
		m_vertex_shader.vertex_shader = nullptr;
	}
}

joj::ErrorCode joj::D3D11Shader::create_vertex_shader()
{
    JOJ_ASSERT(m_device, "Device is nullptr.");
    JOJ_ASSERT(m_vertex_shader.vsblob, "Vertex Shader Bytecode is nullptr.");
    
    if (m_device->device->CreateVertexShader(
		m_vertex_shader.vsblob->GetBufferPointer(),
		m_vertex_shader.vsblob->GetBufferSize(),
		nullptr,
		&m_vertex_shader.vertex_shader) != S_OK)
	{
		JOJ_ERROR(ErrorCode::ERR_SHADER_VERTEX_CREATION,
			"Failed to create Vertex Shader.");
		return ErrorCode::ERR_SHADER_VERTEX_CREATION;
	}

	return ErrorCode::OK;
}

joj::ErrorCode joj::D3D11Shader::create_pixel_shader()
{
    JOJ_ASSERT(m_device, "Device is nullptr.");
    JOJ_ASSERT(m_pixel_shader.psblob, "Pixel Shader Bytecode is nullptr.");

    if (m_device->device->CreatePixelShader(
		m_pixel_shader.psblob->GetBufferPointer(),
		m_pixel_shader.psblob->GetBufferSize(),
		nullptr,
		&m_pixel_shader.pixel_shader) != S_OK)
	{
		JOJ_ERROR(ErrorCode::ERR_SHADER_PIXEL_CREATION,
			"Failed to create Pixel Shader.");
		return ErrorCode::ERR_SHADER_PIXEL_CREATION;
	}

	return ErrorCode::OK;
}

joj::ErrorCode joj::D3D11Shader::create_input_layout(std::vector<InputDesc>& input_desc)
{
    JOJ_ASSERT(m_device, "Device is nullptr.");
    JOJ_ASSERT(m_vertex_shader.vsblob, "Vertex Shader Bytecode is nullptr.");

    std::vector<D3D11_INPUT_ELEMENT_DESC> input_layout;
    for (const InputDesc& desc : input_desc)
    {
        D3D11_INPUT_ELEMENT_DESC d3d11_desc = { 0 };
        d3d11_desc.SemanticName = desc.semantic_name.c_str();
        d3d11_desc.SemanticIndex = desc.semantic_index;
        
        switch (desc.format)
        {
        case DataFormat::R32G32B32A32_FLOAT:
            d3d11_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
            break;
        case DataFormat::R32G32B32_FLOAT:
            d3d11_desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
            break;
        case DataFormat::R32G32_FLOAT:
            d3d11_desc.Format = DXGI_FORMAT_R32G32_FLOAT;
            break;
        case DataFormat::R32_UINT:
            d3d11_desc.Format = DXGI_FORMAT_R32_UINT;
            break;
        case DataFormat::R32G32B32A32_UINT:
            d3d11_desc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
            break;
        case DataFormat::R8G8B8A8_UINT:
            d3d11_desc.Format = DXGI_FORMAT_R8G8B8A8_UINT;
            break;
        default:
            break;
        }

        d3d11_desc.InputSlot = desc.input_slot;
        d3d11_desc.AlignedByteOffset = desc.aligned_byte_offset;

        switch (desc.classification)
        {
        case InputClassification::PerVertexData:
            d3d11_desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
            break;
        case InputClassification::PerInstanceData:
            d3d11_desc.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
            break;
        default:
            break;
        }

        d3d11_desc.InstanceDataStepRate = desc.instance_data_step_rate;

        input_layout.push_back(d3d11_desc);
    }

    if (m_device->device->CreateInputLayout(input_layout.data(),
        static_cast<u32>(input_layout.size()),
        m_vertex_shader.vsblob->GetBufferPointer(),
        m_vertex_shader.vsblob->GetBufferSize(),
        &m_input_layout.input_layout) != S_OK)
    {
        JOJ_ERROR(ErrorCode::ERR_SHADER_INPUT_LAYOUT_CREATION,
            "Failed to create D3D11 Input Layout");
        return ErrorCode::ERR_SHADER_INPUT_LAYOUT_CREATION;
    }

    return ErrorCode::OK;
}

joj::InputLayoutData* joj::D3D11Shader::get_input_layout()
{
    return &m_input_layout;
}

joj::VertexShader* joj::D3D11Shader::get_vertex_shader()
{
    return &m_vertex_shader;
}

joj::PixelShader* joj::D3D11Shader::get_pixel_shader()
{
    return &m_pixel_shader;
}

#endif // JOJ_PLATFORM_WINDOWS