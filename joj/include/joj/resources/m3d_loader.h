#ifndef _JOJ_M3D_LOADER_H
#define _JOJ_M3D_LOADER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include <string>
#include <vector>
#include "renderer/vertex.h"
#include "submesh.h"
#include "systems/material/material.h"
#include "error_code.h"
#include "skinned_data.h"

namespace joj
{
    struct M3DMaterial
    {
        Material mat;
        b8 alpha_clip;
        std::string effect_type_name;
        std::wstring diffuse_map_name;
        std::wstring normal_map_name;
    };

    namespace M3DLoader
    {
        JAPI ErrorCode load_M3D_static(const std::string& filename,
            std::vector<Vertex::PosNormalTexTan>& vertices,
            std::vector<u16>& indices,
            std::vector<SubMesh>& submeshes,
            std::vector<M3DMaterial>& mats);

        JAPI ErrorCode load_M3D_skinned(const std::string& filename,
            std::vector<Vertex::PosNormalTexTanSkinned>& vertices,
            std::vector<u16>& indices,
            std::vector<SubMesh>& submeshes,
            std::vector<M3DMaterial>& mats,
            SkinnedData& skin_info);
    }
}

#endif // _JOJ_M3D_LOADER_H