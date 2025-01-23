#ifndef _JOJ_SCENE_H
#define _JOJ_SCENE_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "systems/camera/camera.h"
#include <vector>
#include "renderer/sprite.h"

namespace joj
{
    class JAPI Scene
    {
    public:
        Scene();
        virtual ~Scene();

        virtual void init() = 0;
        virtual void update(const f32 dt) = 0;
        virtual void draw() = 0;
        virtual void shutdown() = 0;
    
    protected:
        Camera* m_camera;
        std::vector<Sprite*> m_sprites;
    };
}

#endif // _JOJ_SCENE_H