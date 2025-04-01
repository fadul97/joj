#ifndef _JOJ_GLTF_MODEL_H
#define _JOJ_GLTF_MODEL_H

#include "joj/core/defines.h"

#include <vector>
#include "gltf_mesh.h"
#include "joj/resources/scene_node.h"
#include <iostream>
#include <memory>
#include "joj/renderer/d3d11/vertex_buffer_d3d11.h"
#include "joj/renderer/d3d11/index_buffer_d3d11.h"

namespace joj
{
    struct JOJ_API GLTFModel
    {
        std::vector<GLTFMesh> meshes;  // Todas as partes do modelo
        std::vector<SceneNode> nodes;  // Estrutura hierárquica da cena
        std::vector<i32> root_nodes;   // Índices dos nós raiz (GLTFScene usa isso)
        std::vector<std::unique_ptr<D3D11VertexBuffer>> vbs;
        std::vector<std::unique_ptr<D3D11IndexBuffer>> ibs;

        void print_info() const // Apenas para debug
        {
            std::cout << "GLTFModel: " << meshes.size() << " meshes, " 
                    << nodes.size() << " nodes, " 
                    << root_nodes.size() << " root nodes\n";
        }
    };
}

#endif // _JOJ_GLTF_MODEL_H