#ifndef _JOJ_SCENE_H
#define _JOJ_SCENE_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "renderer/renderer.h"
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

        virtual void init(const GraphicsDevice& device, Camera& camera) = 0;
        virtual void update(const f32 dt) = 0;
        virtual void draw(IRenderer& renderer) = 0;
        virtual void shutdown() = 0;

        void add_sprite(Sprite* sprite);
    
    protected:
        Camera* m_camera;
        std::vector<Sprite*> m_sprites;
    };

    inline void Scene::add_sprite(Sprite* sprite)
    { m_sprites.push_back(sprite); }
}

#endif // _JOJ_SCENE_H