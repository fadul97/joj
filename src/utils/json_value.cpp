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

joj::JsonValue& joj::JsonValue::operator[](const std::string& key)
{
    if (std::holds_alternative<Object>(m_value))
    {
        auto& obj = std::get<Object>(m_value);
        return obj[key];
    }

    // Static object to return if the index is out of bounds
    static JsonValue default_value;
    return default_value;
}

const joj::JsonValue& joj::JsonValue::operator[](const std::string& key) const
{
    if (std::holds_alternative<Object>(m_value))
    {
        auto& obj = std::get<Object>(m_value);
        auto it = obj.find(key);

        if (it != obj.end())
            return it->second;
    }

    return JsonValue();
}

joj::JsonValue& joj::JsonValue::operator[](const size_t index)
{
    if (std::holds_alternative<Array>(m_value))
    {
        auto& arr = std::get<Array>(m_value);

        if (index < arr.size())
            return arr[index];
    }

    // Static object to return if the index is out of bounds
    static JsonValue default_value;
    return default_value;
}

const joj::JsonValue& joj::JsonValue::operator[](const size_t index) const
{
    if (std::holds_alternative<Array>(m_value))
    {
        auto& arr = std::get<Array>(m_value);
        
        if (index < arr.size())
        return arr[index];
    }
    
    // Static object to return if the index is out of bounds
    static JsonValue default_value;
    return default_value;
}

std::string joj::JsonValue::as_string() const
{
    if (std::holds_alternative<std::string>(m_value))
        return std::get<std::string>(m_value);

    return "";
}

f64 joj::JsonValue::as_number() const
{
    if (std::holds_alternative<f64>(m_value))
        return std::get<f64>(m_value);

    return JSON_NULL;
}

b8 joj::JsonValue::as_bool() const
{
    if (std::holds_alternative<b8>(m_value))
        return std::get<b8>(m_value);

    return false;
}

const joj::JsonValue::Array& joj::JsonValue::as_array() const
{
    if (std::holds_alternative<Array>(m_value))
        return std::get<Array>(m_value);

    return Array();
}

const joj::JsonValue::Object& joj::JsonValue::as_object() const
{
    if (std::holds_alternative<Object>(m_value))
        return std::get<Object>(m_value);

    return Object();
}

b8 joj::JsonValue::is_object() const
{
    return std::holds_alternative<Object>(m_value);
}

b8 joj::JsonValue::is_array() const
{
    return std::holds_alternative<Array>(m_value);
}

b8 joj::JsonValue::is_null() const
{
    return std::holds_alternative<std::nullptr_t>(m_value);
}

b8 joj::JsonValue::is_string() const
{
    return std::holds_alternative<std::string>(m_value);
}

b8 joj::JsonValue::is_number() const
{
    return std::holds_alternative<f64>(m_value);
}

b8 joj::JsonValue::has_key(const std::string& key) const
{
    if (auto obj = std::get_if<Object>(&m_value))
        return obj->find(key) != obj->end();

    return false;
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