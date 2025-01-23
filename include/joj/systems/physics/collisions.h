#ifndef _JOJ_COLLISIONS_2D_H
#define _JOJ_COLLISIONS_2D_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "defines.h"

#include "rect.h"

namespace joj
{
    b8 on_rect_collision(Rect& rect1, Rect& rect2)
    {
        // Verificar sobreposição no eixo X
        b8 overlapX = rect1.get_right() >= rect2.get_left() &&
            rect1.get_left() <= rect2.get_right();

        // Verificar sobreposição no eixo Y
        b8 overlapY = rect1.get_bottom() >= rect2.get_top() &&
            rect1.get_top() <= rect2.get_bottom();

        // Existe colisão se há sobreposição nos dois eixos
        return overlapX && overlapY;
    }
}

#endif // _JOJ_COLLISIONS_2D_H