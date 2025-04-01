#include "joj/resources/interpolation_type.h"

const char* joj::interpolation_type_to_string(const InterpolationType type)
{
    switch (type)
    {
        case InterpolationType::LINEAR:       return "LINEAR";
        case InterpolationType::STEP:         return "STEP";
        case InterpolationType::CUBICSPLINE:  return "CUBICSPLINE";
        default:                              return "UNKNOWN";
    }
}