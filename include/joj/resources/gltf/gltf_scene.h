#ifndef _JOJ_GLTF_SCENE_H
#define _JOJ_GLTF_SCENE_H

#include "joj/core/defines.h"

#include <string>
#include <vector>
#include "joj/renderer/vertex.h"
#include "joj/resources/submesh.h"

namespace joj
{
    class JOJ_API GLTFScene
    {
    public:
        GLTFScene();
        ~GLTFScene();

        void set_name(const std::string& name);
        void set_vertices(const std::vector<Vertex::ColorTanPosNormalTex>& vertices);
        void set_indices(const std::vector<u16>& indices);
        
        const std::string& get_name() const;
        const std::vector<Vertex::ColorTanPosNormalTex>& get_vertices() const;
        const std::vector<u16>& get_indices() const;

    private:
        std::string m_name;
        std::vector<Vertex::ColorTanPosNormalTex> m_vertices;
        std::vector<u16> m_indices;
    };
}

#endif // _JOJ_GLTF_SCENE_H