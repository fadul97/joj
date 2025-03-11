#ifndef _JOJ_COLLISIONS_2D_H
#define _JOJ_COLLISIONS_2D_H

#include "core/defines.h"

#include "rect.h"

namespace joj
{
    b8 on_rect_collision(Rect& rect1, Rect& rect2)
    {
        // Verificar sobreposi��o no eixo X
        b8 overlapX = rect1.get_right() >= rect2.get_left() &&
            rect1.get_left() <= rect2.get_right();

        // Verificar sobreposi��o no eixo Y
        b8 overlapY = rect1.get_bottom() >= rect2.get_top() &&
            rect1.get_top() <= rect2.get_bottom();

        // Existe colis�o se h� sobreposi��o nos dois eixos
        return overlapX && overlapY;
    }
}

#endif // _JOJ_COLLISIONS_2D_H