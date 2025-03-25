#include "joj/utils/json_value.h"

joj::JsonValue::JsonValue()
    : m_value(nullptr)
{
}

joj::JsonValue::JsonValue(const b8 value)
    : m_value(value)
{
}

joj::JsonValue::JsonValue(const f64 value)
    : m_value(value)
{
}

joj::JsonValue::JsonValue(const std::string& value)
    : m_value(value)
{
}

joj::JsonValue::JsonValue(const Object& value)
    : m_value(value)
{
}

joj::JsonValue::JsonValue(const Array& value)
    : m_value(value)
{
}

b8 joj::JsonValue::is_object() const
{
    return std::holds_alternative<Object>(m_value);
}

b8 joj::JsonValue::is_array() const
{
    return std::holds_alternative<Array>(m_value);
}