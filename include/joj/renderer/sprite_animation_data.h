#ifndef _JOJ_SPRITE_ANIMATION_DATA_H
#define _JOJ_SPRITE_ANIMATION_DATA_H

#include "joj/core/defines.h"

#include "joj/core/jstring.h"
#include "joj/core/vector.h"

namespace joj
{

    struct JOJ_API SpriteAnimationData
    {
        JString name;                 // Nome da anima��o (ex: "Run", "Idle").
        JVectorI32 frames;            // Lista dos �ndices dos quadros.
        f32 frame_duration{ 0.0f };    // Tempo que cada quadro deve ser exibido.
    };
}

#endif // _JOJ_SPRITE_ANIMATION_DATA_H