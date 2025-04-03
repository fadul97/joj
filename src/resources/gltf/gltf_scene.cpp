#include "joj/resources/gltf/gltf_scene.h"

joj::GLTFScene::GLTFScene()
{
}

joj::GLTFScene::~GLTFScene()
{
}

void joj::GLTFScene::set_name(const std::string& name)
{
    m_name = name;
}

void joj::GLTFScene::set_vertices(const std::vector<Vertex::ColorTanPosNormalTex>& vertices)
{
    m_vertices = vertices;
}

void joj::GLTFScene::set_indices(const std::vector<u16>& indices)
{
    m_indices = indices;
}

const std::string& joj::GLTFScene::get_name() const
{
    return m_name;
}

const std::vector<joj::Vertex::ColorTanPosNormalTex>& joj::GLTFScene::get_vertices() const
{
    return m_vertices;
}

const std::vector<u16>& joj::GLTFScene::get_indices() const
{
    return m_indices;
}