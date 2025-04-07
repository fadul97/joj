#ifndef _JOJ_SCENE_NODE_H
#define _JOJ_SCENE_NODE_H

#include "joj/core/defines.h"

#include <string>
#include <vector>

#include "joj/core/math/vector3.h"	
#include "joj/core/math/vector4.h"
#include "joj/core/math/matrix4x4.h"

#include "joj/resources/mesh.h"

namespace joj
{
    class JOJ_API SceneNode
    {
    public:
        SceneNode();
        SceneNode(const std::string& name);
        SceneNode(const std::string& name, const i32 parent_index);
        ~SceneNode();

        void set_name(const char* name);
        void set_position(const Vector3& pos);
        void set_rotation(const Vector4& rot);
        void set_scale(const Vector3& scl);
        void set_mesh(const Mesh* mesh);

        void add_child(const SceneNode* child_node);
        void set_parent(const SceneNode* parent_node);

        std::string get_name() const;
        Vector3 get_position() const;
        Vector4 get_rotation() const;
        Vector3 get_scale() const;
        const Mesh* get_mesh() const;
        const std::vector<const SceneNode*>& get_children() const; // FIXME: Avoid copying the vector
        const SceneNode* get_parent() const;

    private:
        std::string m_name;

        Matrix4x4 m_local_matrix;
        Vector3 m_position;
        Vector4 m_rotation;
        Vector3 m_scale;

        const Mesh* m_mesh;
        const SceneNode* m_parent;

        std::vector<const SceneNode*> m_children;
    };
}

#endif // _JOJ_SCENE_NODE_H