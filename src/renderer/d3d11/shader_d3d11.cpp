#include "renderer/d3d11/shader_d3d11.h"

#if JPLATFORM_WINDOWS

#include <d3dcompiler.h>
#include "logger.h"
#include "renderer/d3d11/renderer_d3d11.h"

joj::D3D11Shader::D3D11Shader()
{
    m_vertex_shader.vertex_shader = nullptr;
    m_vertex_shader.vsblob = nullptr;
    
    m_pixel_shader.pixel_shader = nullptr;
    m_pixel_shader.psblob = nullptr;
    
    m_geometry_shader.geometry_shader = nullptr;
    m_geometry_shader.gsblob = nullptr;
    
    m_compute_shader.compute_shader = nullptr;
    m_compute_shader.csblob = nullptr;
}

joj::D3D11Shader::~D3D11Shader()
{
	// Relase Direct3D resources
	if (m_pixel_shader.psblob)
	{
		m_pixel_shader.psblob->Release();
		m_pixel_shader.psblob = nullptr;
	}

	if (m_compute_shader.csblob)
	{
		m_compute_shader.csblob->Release();
		m_compute_shader.csblob = nullptr;
	}

	if (m_geometry_shader.gsblob)
	{
		m_geometry_shader.gsblob->Release();
		m_geometry_shader.gsblob = nullptr;
	}

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

	// Relase Compute Shader
	if (m_compute_shader.compute_shader)
	{
		m_compute_shader.compute_shader->Release();
		m_compute_shader.compute_shader = nullptr;
	}

	// Relase Geometry Shader
	if (m_geometry_shader.geometry_shader)
	{
		m_geometry_shader.geometry_shader->Release();
		m_geometry_shader.geometry_shader = nullptr;
	}

	// Relase Vertex Shader
	if (m_vertex_shader.vertex_shader)
	{
		m_vertex_shader.vertex_shader->Release();
		m_vertex_shader.vertex_shader = nullptr;
	}
}

void joj::D3D11Shader::compile_vertex_shader(const std::string& vertex_shader,
	const std::string& entry_point, const ShaderModel shader_model)
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
		model = "vs_5_0";
		break;
	default:
		model = "vs_5_0";
		break;
	}

	if (D3DCompile(
		vertex_shader.c_str(), vertex_shader.length(), nullptr, nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, entry_point.c_str(), model.c_str(),
		shader_flags, 0, &m_vertex_shader.vsblob, &error_blob) != S_OK)
	{
		if (error_blob)
		{
			OutputDebugStringA((char*)error_blob->GetBufferPointer());
			JERROR(ErrorCode::ERR_SHADER_D3D11_VERTEX_COMPILATION, "%s",
				(char*)error_blob->GetBufferPointer());
			error_blob->Release();
		}
	}

	if (error_blob)
		error_blob->Release();
}

// Compile from memory
void joj::D3D11Shader::compile_pixel_shader(const std::string& pixel_shader,
	const std::string& entry_point, const ShaderModel shader_model)
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
		D3D_COMPILE_STANDARD_FILE_INCLUDE, entry_point.c_str(), model.c_str(),
		shader_flags, 0, &m_pixel_shader.psblob, &error_blob) != S_OK)
	{
		if (error_blob)
		{
			OutputDebugStringA((char*)error_blob->GetBufferPointer());
			JERROR(ErrorCode::ERR_SHADER_D3D11_PIXEL_COMPILATION, "%s",
				(char*)error_blob->GetBufferPointer());
			error_blob->Release();
		}
	}

	if (error_blob)
		error_blob->Release();
}

void joj::D3D11Shader::compile_vertex_shader_from_file(const std::string& vertex_path,
    const std::string& entry_point, const ShaderModel shader_model)
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

	ID3DBlob* shader_compile_errors_blob = nullptr;    // To get info about compilation

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
		D3D_COMPILE_STANDARD_FILE_INCLUDE, entry_point.c_str(),
		model.c_str(), shader_flags, NULL, &m_vertex_shader.vsblob,
		&shader_compile_errors_blob) != S_OK)
	{
		if (shader_compile_errors_blob != nullptr)
		{
			OutputDebugStringA((char*)shader_compile_errors_blob->GetBufferPointer());
			JERROR(ErrorCode::ERR_SHADER_D3D11_VERTEX_COMPILATION, "%s",
				(char*)shader_compile_errors_blob->GetBufferPointer());
			shader_compile_errors_blob->Release();
		}
		else
		{
			JERROR(ErrorCode::ERR_SHADER_D3D11_VERTEX_COMPILATION,
				"Failed to compile Vertex Shader.");
		}
	}

	if (shader_compile_errors_blob != nullptr)
	{
		shader_compile_errors_blob->Release();
	}
}

void joj::D3D11Shader::compile_pixel_shader_from_file(const std::string& pixel_path,
    const std::string& entry_point, const ShaderModel shader_model)
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

	ID3DBlob* shader_compile_errors_blob = nullptr;    // To get info about compilation

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
		D3D_COMPILE_STANDARD_FILE_INCLUDE, entry_point.c_str(),
		model.c_str(), shader_flags, NULL, &m_pixel_shader.psblob,
		&shader_compile_errors_blob) != S_OK)
	{
		if (shader_compile_errors_blob != nullptr)
		{
			OutputDebugStringA((char*)shader_compile_errors_blob->GetBufferPointer());
			JERROR(ErrorCode::ERR_SHADER_D3D11_PIXEL_COMPILATION, "%s",
				(char*)shader_compile_errors_blob->GetBufferPointer());
			shader_compile_errors_blob->Release();
		}
		else
		{
			JERROR(ErrorCode::ERR_SHADER_D3D11_PIXEL_COMPILATION,
				"Failed to compile Pixel Shader.");
		}
	}

	if (shader_compile_errors_blob != nullptr)
	{
		shader_compile_errors_blob->Release();
	}
}

void joj::D3D11Shader::compile_geometry_shader_from_file(const std::string& geometry_path,
    const std::string& entry_point, const ShaderModel shader_model)
{
	DWORD shader_flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef JOJ_DEBUG_MODE
	// Let compiler insert debug information into the output code
	shader_flags |= D3DCOMPILE_DEBUG;

	// Disable optimizations
	shader_flags |= D3DCOMPILE_SKIP_OPTIMIZATION;      // Compiler will not validate the generated code -> Recommended to use only with successfully compiled shaders
#endif // JOJ_DEBUG_MODE

	// --------------------------------
	// Geometry Shader
	// --------------------------------

	ID3DBlob* shader_compile_errors_blob = nullptr;    // To get info about compilation

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

	std::wstring wide_path(geometry_path.begin(), geometry_path.end());

	// Compile Vertex Shader
	if (D3DCompileFromFile(wide_path.c_str(), nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, entry_point.c_str(),
		model.c_str(), shader_flags, NULL, &m_geometry_shader.gsblob,
		&shader_compile_errors_blob) != S_OK)
	{
		OutputDebugStringA((char*)shader_compile_errors_blob->GetBufferPointer());
		JERROR(ErrorCode::ERR_SHADER_D3D11_GEOMETRY_COMPILATION, "%s",
			(char*)shader_compile_errors_blob->GetBufferPointer());
		shader_compile_errors_blob->Release();
	}

	if (shader_compile_errors_blob != nullptr)
	{
		shader_compile_errors_blob->Release();
	}
}

void joj::D3D11Shader::compile_compute_shader_from_file(const std::string& compute_path,
    const std::string& entry_point, const ShaderModel shader_model)
{
	DWORD shader_flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef JOJ_DEBUG_MODE
	// Let compiler insert debug information into the output code
	shader_flags |= D3DCOMPILE_DEBUG;

	// Disable optimizations
	shader_flags |= D3DCOMPILE_SKIP_OPTIMIZATION;      // Compiler will not validate the generated code -> Recommended to use only with successfully compiled shaders
#endif // JOJ_DEBUG_MODE

	// --------------------------------
	// Compute Shader
	// --------------------------------

	ID3DBlob* shader_compile_errors_blob = nullptr;    // To get info about compilation

	std::string model;
	switch (shader_model)
	{
	case ShaderModel::Default:
		model = "cs_5_0";
		break;
	default:
		model = "cs_5_0";
		break;
	}

	std::wstring wide_path(compute_path.begin(), compute_path.end());

	// Compile Vertex Shader
	if (D3DCompileFromFile(wide_path.c_str(), nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, entry_point.c_str(),
		model.c_str(), shader_flags, NULL, &m_compute_shader.csblob,
		&shader_compile_errors_blob) != S_OK)
	{
		OutputDebugStringA((char*)shader_compile_errors_blob->GetBufferPointer());
		JERROR(ErrorCode::ERR_SHADER_D3D11_COMPUTE_COMPILATION, "%s",
			(char*)shader_compile_errors_blob->GetBufferPointer());
		shader_compile_errors_blob->Release();
	}

	if (shader_compile_errors_blob != nullptr)
	{
		shader_compile_errors_blob->Release();
	}
}

joj::ErrorCode joj::D3D11Shader::create_vertex_shader(const GraphicsDevice& device)
{
	if (device.device->CreateVertexShader(
		m_vertex_shader.vsblob->GetBufferPointer(),
		m_vertex_shader.vsblob->GetBufferSize(),
		nullptr,
		&m_vertex_shader.vertex_shader) != S_OK)
	{
		JERROR(ErrorCode::ERR_SHADER_D3D11_VERTEX_CREATION,
			"Failed to create Vertex Shader.");
		return ErrorCode::ERR_SHADER_D3D11_VERTEX_CREATION;
	}

	return ErrorCode::OK;
}

joj::ErrorCode joj::D3D11Shader::create_pixel_shader(const GraphicsDevice& device)
{
	if (device.device->CreatePixelShader(
		m_pixel_shader.psblob->GetBufferPointer(),
		m_pixel_shader.psblob->GetBufferSize(),
		nullptr,
		&m_pixel_shader.pixel_shader) != S_OK)
	{
		JERROR(ErrorCode::ERR_SHADER_D3D11_PIXEL_CREATION,
			"Failed to create Pixel Shader.");
		return ErrorCode::ERR_SHADER_D3D11_PIXEL_CREATION;
	}

	return ErrorCode::OK;
}

void joj::D3D11Shader::bind_vertex_shader(CommandList& cmd_list)
{
	cmd_list.device_context->VSSetShader(m_vertex_shader.vertex_shader, nullptr, 0);
}

void joj::D3D11Shader::bind_pixel_shader(CommandList& cmd_list)
{
	cmd_list.device_context->PSSetShader(m_pixel_shader.pixel_shader, nullptr, 0);
}

joj::VertexShader& joj::D3D11Shader::get_vertex_shader()
{
	return m_vertex_shader;
}

joj::PixelShader& joj::D3D11Shader::get_pixel_shader()
{
	return m_pixel_shader;
}

joj::GeometryShader& joj::D3D11Shader::get_geometry_shader()
{
	return m_geometry_shader;
}

joj::ComputeShader& joj::D3D11Shader::get_compute_shader()
{
	return m_compute_shader;
}

#endif // JPLATFORM_WINDOWS