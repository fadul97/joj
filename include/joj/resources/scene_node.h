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

        void set_position(const Vector3& pos);
        void set_rotation(const Vector4& rot);
        void set_scale(const Vector3& scl);

        void add_child(const i32& child_index);
        void set_parent(const i32& parent_index);

    private:
        std::string m_name;

        Vector3 m_position;
        Vector4 m_rotation;
        Vector3 m_scale;

        std::vector<i32> m_children_indices;
        i32 m_parent_index;
    };
}

#endif // _JOJ_SCENE_NODE_H