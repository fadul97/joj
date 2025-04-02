#ifndef _JOJ_GLTF_MODEL_H
#define _JOJ_GLTF_MODEL_H

#include "joj/core/defines.h"

#include <vector>
#include "gltf_mesh.h"
#include "gltf_node.h"
#include "joj/resources/mesh.h"
#include <iostream>
#include <memory>
#include "joj/renderer/d3d11/vertex_buffer_d3d11.h"
#include "joj/renderer/d3d11/index_buffer_d3d11.h"
#include <unordered_map>

namespace joj
{
    struct JOJ_API GLTFModel
    {
        std::vector<GLTFMesh> meshes;  // Todas as partes do modelo
        std::vector<GLTFNode> nodes;  // Estrutura hierárquica da cena
        std::vector<i32> root_nodes;   // Índices dos nós raiz (GLTFScene usa isso)
        std::vector<std::unique_ptr<D3D11VertexBuffer>> vbs;
        std::vector<std::unique_ptr<D3D11IndexBuffer>> ibs;
        std::vector<Mesh> aggregated_meshes;

        void print_info() const // Apenas para debug
        {
            std::cout << "GLTFModel: " << meshes.size() << " meshes, " 
                    << nodes.size() << " nodes, " 
                    << root_nodes.size() << " root nodes\n";
        }
    };

    inline void print_model_hierarchy(const GLTFModel& model)
    {
        std::cout << "=== GLTF Model Hierarchy ===\n";
    
        // Criar um mapa para descobrir o pai de cada nó
        std::unordered_map<i32, i32> parent_map;
        
        for (size_t i = 0; i < model.nodes.size(); ++i)
        {
            const auto& node = model.nodes[i];
            for (i32 child_index : node.children)
            {
                parent_map[child_index] = static_cast<i32>(i);
            }
        }

        // Percorrer os nós para imprimir a relação pai-filho
        for (size_t i = 0; i < model.nodes.size(); ++i)
        {
            const auto& node = model.nodes[i];
            std::cout << "Node " << i << " - Name: " << node.name;

            // Verificar se o nó tem um pai
            if (parent_map.find(i) != parent_map.end())
            {
                std::cout << " (Parent: " << parent_map[i] << " - " << model.nodes[parent_map[i]].name << ")";
            }
            else
            {
                std::cout << " (Root Node)";
            }

            std::cout << "\n";

            // Imprimir os filhos
            if (!node.children.empty())
            {
                std::cout << "  Children: ";
                for (i32 child_index : node.children)
                {
                    std::cout << child_index << " (" << model.nodes[child_index].name << "), ";
                }
                std::cout << "\n";
            }
        }
        std::cout << "===========================\n";
    }
}

#endif // _JOJ_GLTF_MODEL_H