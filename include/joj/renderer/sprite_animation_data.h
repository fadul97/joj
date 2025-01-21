#ifndef _JOJ_SPRITE_ANIMATION_DATA_H
#define _JOJ_SPRITE_ANIMATION_DATA_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include <string>
#include <vector>

namespace joj
{
    struct SpriteAnimationData
    {
        std::string name;         // Nome da animação (ex: "Run", "Idle").
        std::vector<i32> frames;  // Lista dos índices dos quadros.
        f32 frameDuration;        // Tempo que cada quadro deve ser exibido.
    };
}

#endif // _JOJ_SPRITE_ANIMATION_DATA_H