#ifndef _JOJ_SPRITE_ANIMATION_DATA_H
#define _JOJ_SPRITE_ANIMATION_DATA_H

#include "core/defines.h"

#include "core/jstring.h"
#include "core/vector.h"

namespace joj
{

    struct JAPI SpriteAnimationData
    {
        JString name;                 // Nome da anima��o (ex: "Run", "Idle").
        JVectorI32 frames;            // Lista dos �ndices dos quadros.
        f32 frame_duration{ 0.0f };    // Tempo que cada quadro deve ser exibido.
    };
}

#endif // _JOJ_SPRITE_ANIMATION_DATA_H