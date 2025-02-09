#include "resources/basic_model.h"

#include "resources/m3d_loader.h"
#include "core/jmacros.h"

joj::BasicModel::BasicModel()
    : m_submesh_count{ 0 }, m_mesh{ nullptr }
{
}

joj::BasicModel::~BasicModel()
{
}