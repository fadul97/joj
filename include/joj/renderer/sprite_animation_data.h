#ifndef _JOJ_SPRITE_ANIMATION_DATA_H
#define _JOJ_SPRITE_ANIMATION_DATA_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "core/defines.h"

#include "core/jstring.h"
#include "core/jvector.h"

namespace joj
{

    struct JAPI SpriteAnimationData
    {
        JString name;                 // Nome da animação (ex: "Run", "Idle").
        JVectorI32 frames;            // Lista dos índices dos quadros.
        f32 frame_duration{ 0.0f };    // Tempo que cada quadro deve ser exibido.
    };
}

#endif // _JOJ_SPRITE_ANIMATION_DATA_H