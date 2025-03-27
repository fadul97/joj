#include "joj/resources/data_type.h"

const char* joj::data_type_to_string(const DataType type)
{
    switch (type)
    {
    case DataType::VEC2:    return "VEC2";
    case DataType::VEC3:    return "VEC3";
    case DataType::VEC4:    return "VEC4";
    case DataType::SCALAR:  return "SCALAR";
    case DataType::MAT2:    return "MAT2";
    case DataType::MAT3:    return "MAT3";
    case DataType::MAT4:    return "MAT4";
    case DataType::UNKNOWN: return "UNKNOWN";
    default:                return "UNKNOWN";
    }
}