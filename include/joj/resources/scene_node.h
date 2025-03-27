#ifndef _JOJ_SCENE_NODE_H
#define _JOJ_SCENE_NODE_H

#include "joj/core/defines.h"

#include <string>
#include <vector>

#include "joj/core/math/vector3.h"	
#include "joj/core/math/vector4.h"

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
        void set_mesh(const i32 mesh_index);
        void set_skin(const i32 skin_index);

        void add_child(const i32& child_index);
        void set_parent(const i32& parent_index);

        std::string get_name() const;
        Vector3 get_position() const;
        Vector4 get_rotation() const;
        Vector3 get_scale() const;
        i32 get_mesh() const;
        i32 get_skin() const;
        std::vector<i32> get_children_indices() const; // FIXME: Avoid copying the vector
        i32 get_parent_index() const;

    private:
        std::string m_name;

        Vector3 m_position;
        Vector4 m_rotation;
        Vector3 m_scale;

        i32 m_mesh;
        i32 m_skin;

        std::vector<i32> m_children_indices;
        i32 m_parent_index;
    };
}

#endif // _JOJ_SCENE_NODE_H