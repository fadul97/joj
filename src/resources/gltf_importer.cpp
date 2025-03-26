#include "joj/resources/gltf_importer.h"

joj::GLTFImporter::GLTFImporter()
    : m_gltf_filename(nullptr), m_bin_filename(nullptr)
{
}

joj::GLTFImporter::GLTFImporter(const char* filename)
    : m_gltf_filename(filename), m_bin_filename(nullptr)
{
}

joj::GLTFImporter::~GLTFImporter()
{
}

joj::ErrorCode joj::GLTFImporter::load()
{
    if (m_gltf_filename == nullptr)
        return ErrorCode::FAILED;

    if (!parse_json())
        return ErrorCode::FAILED;

    return ErrorCode::OK;
}

std::vector<joj::GLTFVertex> joj::GLTFImporter::get_vertices()
{
    std::vector<GLTFVertex> vertices;
    return vertices;
}

b8 joj::GLTFImporter::parse_json()
{
    return false;
}