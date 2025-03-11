#include "joj/resources/basic_model.h"

#include "joj/resources/m3d_loader.h"
#include "joj/core/jmacros.h"

joj::BasicModel::BasicModel()
    : m_submesh_count{ 0 }, m_mesh{ nullptr }
{
}

joj::BasicModel::~BasicModel()
{
}