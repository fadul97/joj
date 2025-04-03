#include "joj/utils/json_value.h"

joj::JsonValue::JsonValue()
    : m_value(nullptr)
{
}

joj::JsonValue::JsonValue(const b8 value)
    : m_value(std::make_unique<Value>(value))
{
}

joj::JsonValue::JsonValue(const i32 value)
    : m_value(std::make_unique<Value>(value))
{
}

joj::JsonValue::JsonValue(const f64 value)
    : m_value(std::make_unique<Value>(value))
{
}

joj::JsonValue::JsonValue(const std::string& value)
    : m_value(std::make_unique<Value>(value))
{
}

joj::JsonValue::JsonValue(const Object& value)
    : m_value(std::make_unique<Value>(value))
{
}

joj::JsonValue::JsonValue(const Array& value)
    : m_value(std::make_unique<Value>(value))
{
}

joj::JsonValue::JsonValue(const JsonValue& other)
{
    if (other.m_value)
    {
        m_value = std::make_unique<Value>(*other.m_value);
    }
}

joj::JsonValue& joj::JsonValue::operator=(const JsonValue& other)
{
    if (this != &other)
    {
        if (other.m_value)
        {
            m_value = std::make_unique<Value>(*other.m_value);
        }
        else
        {
            m_value.reset();
        }
    }

    return *this;
}


joj::JsonValue& joj::JsonValue::operator[](const std::string& key)
{
    if (m_value && std::holds_alternative<Object>(*m_value))
    {
        auto& obj = std::get<Object>(*m_value);
        return obj[key];
    }

    // Static object to return if the index is out of bounds
    static JsonValue default_value;
    return default_value;
}

const joj::JsonValue& joj::JsonValue::operator[](const std::string& key) const
{
    if (m_value && std::holds_alternative<Object>(*m_value))
    {
        auto& obj = std::get<Object>(*m_value);
        auto it = obj.find(key);

        if (it != obj.end())
            return it->second;
    }

    // TODO: Log an error?
    static const JsonValue null_value;
    return null_value;
}

joj::JsonValue& joj::JsonValue::operator[](const size_t index)
{
    if (m_value && std::holds_alternative<Array>(*m_value))
    {
        auto& arr = std::get<Array>(*m_value);

        if (index < arr.size())
            return arr[index];
    }

    // Static object to return if the index is out of bounds
    static JsonValue default_value;
    return default_value;
}

const joj::JsonValue& joj::JsonValue::operator[](const size_t index) const
{
    if (m_value && std::holds_alternative<Array>(*m_value))
    {
        auto& arr = std::get<Array>(*m_value);
        
        if (index < arr.size())
        return arr[index];
    }
    
    // Static object to return if the index is out of bounds
    static JsonValue default_value;
    return default_value;
}

size_t joj::JsonValue::size() const
{
    // If the value is valid
    if (m_value)
    {
        // If the value is an object, return the size of the object
        if (std::holds_alternative<Object>(*m_value))
            return std::get<Object>(*m_value).size();
        // If the value is an array, return the size of the array
        else if (std::holds_alternative<Array>(*m_value))
            return std::get<Array>(*m_value).size();
    }

    // Otherwise, return 0
    return 0;
}

std::string joj::JsonValue::as_string() const
{
    if (m_value && std::holds_alternative<std::string>(*m_value))
        return std::get<std::string>(*m_value);

    return "";
}

f64 joj::JsonValue::as_number() const
{
    // If the value is valid
    if (m_value)
    {
        // If the value is a number, return the number
        if (std::holds_alternative<f64>(*m_value))
            return std::get<f64>(*m_value);
        else if (std::holds_alternative<i32>(*m_value))
            return static_cast<f64>(std::get<i32>(*m_value));
    }

    return JSON_NULL;
}

b8 joj::JsonValue::as_bool() const
{
    if (m_value && std::holds_alternative<b8>(*m_value))
        return std::get<b8>(*m_value);

    return false;
}

const joj::JsonValue::Array& joj::JsonValue::as_array() const
{
    if (m_value && std::holds_alternative<Array>(*m_value))
        return std::get<Array>(*m_value);

    // TODO: Log an error?
    static const Array null_array;
    return null_array;
}

const joj::JsonValue::Object& joj::JsonValue::as_object() const
{
    if (m_value && std::holds_alternative<Object>(*m_value))
        return std::get<Object>(*m_value);

    // TODO: Log an error?
    static const Object null_object;
    return null_object;
}

i32 joj::JsonValue::as_int() const
{
    // If the value is valid
    if (m_value)
    {
        if (std::holds_alternative<i32>(*m_value))
            return std::get<i32>(*m_value);
        else if (std::holds_alternative<f64>(*m_value))
            return static_cast<i32>(std::get<f64>(*m_value));
    }
    
    return static_cast<i32>(JSON_NULL);
}

f32 joj::JsonValue::as_float() const
{
    if (m_value)
    {
        if (std::holds_alternative<f64>(*m_value))
            return static_cast<f32>(std::get<f64>(*m_value));
        else if (std::holds_alternative<i32>(*m_value))
            return static_cast<f32>(std::get<i32>(*m_value));
    }
    
    return static_cast<f32>(JSON_NULL);
}

b8 joj::JsonValue::is_object() const
{
    return m_value && std::holds_alternative<Object>(*m_value);
}

b8 joj::JsonValue::is_array() const
{
    return m_value && std::holds_alternative<Array>(*m_value);
}

b8 joj::JsonValue::is_null() const
{
    return m_value && std::holds_alternative<std::nullptr_t>(*m_value);
}

b8 joj::JsonValue::is_string() const
{
    return m_value && std::holds_alternative<std::string>(*m_value);
}

b8 joj::JsonValue::is_number() const
{
    return is_float() || is_int();
}

b8 joj::JsonValue::is_float() const
{
    return m_value && std::holds_alternative<f64>(*m_value);
}

b8 joj::JsonValue::is_int() const
{
    return m_value && std::holds_alternative<i32>(*m_value);
}

b8 joj::JsonValue::has_key(const std::string& key) const
{
    if (m_value)
    {
        if (auto obj = std::get_if<Object>(m_value.get()))
            return obj->find(key) != obj->end();
    }

    return false;
}

void joj::JsonValue::print(std::ostream& os, i32 indent) const
{
    if (m_value)
    {
        std::visit([&](auto&& arg) { print_value(arg, os, indent); }, *m_value);
    }
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

void joj::JsonValue::print_value(const i32& value, std::ostream& os, i32 indent)
{
    os << value;
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