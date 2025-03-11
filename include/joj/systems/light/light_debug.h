#ifndef _JOJ_LIGHT_DEBUG_H
#define _JOJ_LIGHT_DEBUG_H

#include "joj/core/defines.h"

#include "light.h"

namespace joj
{
    JOJ_API b8 are_directional_lights_equals(DirectionalLight& l1, DirectionalLight& l2);
    JOJ_API b8 are_point_lights_equals(PointLight& l1, PointLight& l2);
    JOJ_API b8 are_spot_lights_equals(SpotLight& l1, SpotLight& l2);
}

#endif // _JOJ_LIGHT_DEBUG_H