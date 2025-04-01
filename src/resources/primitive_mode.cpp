#include "joj/resources/primitive_mode.h"

const char* joj::primitive_mode_to_str(const PrimitiveMode& mode)
{
    switch (mode)
    {
        case PrimitiveMode::POINTS:         return "POINTS";
        case PrimitiveMode::LINES:          return "LINES";
        case PrimitiveMode::LINE_LOOP:      return "LINE_LOOP";
        case PrimitiveMode::LINE_STRIP:     return "LINE_STRIP";
        case PrimitiveMode::TRIANGLES:      return "TRIANGLES";
        case PrimitiveMode::TRIANGLE_STRIP: return "TRIANGLE_STRIP";
        case PrimitiveMode::TRIANGLE_FAN:   return "TRIANGLE_FAN";
        default:                            return "UNKNOWN";
    }
}