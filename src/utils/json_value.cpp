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

void joj::JsonValue::print(std::ostream& os, i32 indent) const
{
    std::visit([&](auto&& arg) { print_value(arg, os, indent); }, m_value);
}

void joj::JsonValue::print_indent(std::ostream& os, i32 indent)
{
    for (i32 i = 0; i < indent; ++i)
        os << "  ";
}

void joj::JsonValue::print_value(const std::nullptr_t&, std::ostream& os, i32 indent)
{
    os << "null";
}

void joj::JsonValue::print_value(const b8& value, std::ostream& os, i32 indent)
{
    os << (value ? "true" : "false");
}

void joj::JsonValue::print_value(const f64& value, std::ostream& os, i32 indent)
{
    os << value;
}

void joj::JsonValue::print_value(const std::string& value, std::ostream& os, i32 indent)
{
    os << "\"" << value << "\"";
}

void joj::JsonValue::print_value(const Array& array, std::ostream& os, i32 indent)
{
    os << "[\n";
    for (i32 i = 0; i < array.size(); ++i)
    {
        print_indent(os, indent + 1);
        array[i].print(os, indent + 1);

        if (i < array.size() - 1)
            os << ",";

        os << "\n";
    }

    print_indent(os, indent);
    os << "]";
}

void joj::JsonValue::print_value(const Object& object, std::ostream& os, i32 indent)
{
    os << "{\n";
    i32 i = 0;

    for (const auto& [key, value] : object)
    {
        print_indent(os, indent + 1);
        os << "\"" << key << "\": ";
        value.print(os, indent + 1);

        if (i < object.size() - 1)
            os << ",";

        os << "\n";
        i++;
    }

    print_indent(os, indent);
    os << "}";
}