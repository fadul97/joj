#ifndef _JOJ_JSON_VALUE_H
#define _JOJ_JSON_VALUE_H

#include "joj/core/defines.h"

#include <string>
#include <unordered_map>
#include <vector>
#include <variant>
#include <iostream>

namespace joj
{
    constexpr f64 JSON_NULL = -9999999.99f;

    /**
     * @brief A class that represents a JSON value.
     * It can be a null, boolean, number, string, object or array.
     */
    class JOJ_API JsonValue
    {
    public:
        /**
         * @brief The JSON value types.
         * 
         * Object: A JSON object.
         * Array: A JSON array.
         * Value: A JSON value.
         * 
         */
        // FIXME: Second in std::pair has incomplete type on GCC (obs: works fine on MSVC)
        // Options: Use std::unique_ptr<JsonValue>, instead of JsonValue
        using Object = std::unordered_map<std::string, JsonValue>;
        using Array = std::vector<JsonValue>;
        using Value = std::variant<std::nullptr_t, b8, i32, f64, std::string, Object, Array>;
    
        /**
         * @brief Construct a new Json Value object with a null value.
         */
        JsonValue();

        /**
         * @brief Construct a new Json Value object with a boolean value.
         * 
         * @param value The boolean value.
         */
        JsonValue(const b8 value);

        /**
         * @brief Construct a new Json Value object with an integer value.
         * 
         * @param value The number value.
         */
        JsonValue(const i32 value);

        /**
         * @brief Construct a new Json Value object with a number value.
         * 
         * @param value The number value.
         */
        JsonValue(const f64 value);

        /**
         * @brief Construct a new Json Value object with a string value.
         * 
         * @param value The string value.
         */
        JsonValue(const std::string& value);

        /**
         * @brief Construct a new Json Value object with an object value.
         * 
         * @param value The object value.
         */
        JsonValue(const Object& value);

        /**
         * @brief Construct a new Json Value object with an array value.
         * 
         * @param value The array value.
         */
        JsonValue(const Array& value);

        /**
         * @brief Access the JSON value by key only if it is an object.
         * 
         * @param key The key.
         * @return JsonValue& The JSON value.
         */
        JsonValue& operator[](const std::string& key);

        /**
         * @brief Access the JSON value by key only if it is an object.
         * 
         * @param key The key.
         * @return const JsonValue& The JSON value.
         */
        const JsonValue& operator[](const std::string& key) const;

        /**
         * @brief Access the JSON value by index only if it is an array.
         * 
         * @param index The index.
         * @return JsonValue& The JSON value.
         */
        JsonValue& operator[](const size_t index);

        /**
         * @brief Access the JSON value by index only if it is an array.
         * 
         * @param index The index.
         * @return const JsonValue& The JSON value.
         */
        const JsonValue& operator[](const size_t index) const;

        /**
         * @brief Get the size of the JSON value.
         * 
         * @return size_t The size of the JSON value.
         */
        size_t size() const;

        /**
         * @brief Get the JSON value as a string.
         * 
         * @return std::string The JSON value as a string.
         */
        std::string as_string() const;
        
        /**
         * @brief Get the JSON value as a number.
         * 
         * @return f64 The JSON value as a number.
         */
        f64 as_number() const;
        
        /**
         * @brief Get the JSON value as a boolean.
         * 
         * @return b8 The JSON value as a boolean.
         */
        b8 as_bool() const;

        /**
         * @brief Get the JSON value as an array.
         * 
         * @return const Array& The JSON value as an array.
         */
        const Array& as_array() const;

        /**
         * @brief Get the JSON value as an object.
         * 
         * @return const Object& The JSON value as an object.
         */
        const Object& as_object() const;

        /**
         * @brief Get the JSON value as an integer.
         * 
         * @return i32 The JSON value as an integer.
         */
        i32 as_int() const;

        /**
         * @brief Get the JSON value as a float.
         * 
         * @return f32 The JSON value as a float.
         */
        f32 as_float() const;
    
        /**
         * @brief Check if the JSON value is an object.
         * 
         * @return b8 True if the JSON value is an object, false otherwise.
         */
        b8 is_object() const;

        /**
         * @brief Check if the JSON value is an array.
         * 
         * @return b8 True if the JSON value is an array, false otherwise.
         */
        b8 is_array() const;

        /**
         * @brief Check if the JSON value is null.
         * 
         * @return b8 True if the JSON value is null, false otherwise.
         */
        b8 is_null() const;

        /**
         * @brief Check if the JSON value is a string.
         * 
         * @return b8 True if the JSON value is a string, false otherwise.
         */
        b8 is_string() const;

        /**
         * @brief Check if the JSON value is a number.
         * 
         * @return b8 True if the JSON value is a number, false otherwise.
         */
        b8 is_number() const;

        /**
         * @brief Check if the JSON value is a float.
         * 
         * @return b8 True if the JSON value is a float, false otherwise.
         */
        b8 is_float() const;

        /**
         * @brief Check if the JSON value is an integer.
         * 
         * @return b8 True if the JSON value is an integer, false otherwise.
         */
        b8 is_int() const;

        /**
         * @brief Check if the JSON value has key.
         *
         * @param key The key to check.
         * @return b8 True if the JSON value has the key, false otherwise.
         */
        b8 has_key(const std::string& key) const;

        /**
         * @brief Print the JSON value recursively.
         * 
         * @param os The output stream.
         * @param indent The indentation.
         */
        void print(std::ostream& os = std::cout, i32 indent = 0) const;

    private:
        /** @brief The JSON value. */
        Value m_value;

        /**
         * @brief Print the JSON value.
         * 
         * @param os The output stream.
         * @param indent The indentation.
         */
        static void print_indent(std::ostream& os, i32 indent);
    
        /**
         * @brief Print the JSON value.
         * 
         * @param value The JSON value.
         * @param os The output stream.
         * @param indent The indentation.
         */
        static void print_value(const std::nullptr_t&, std::ostream& os, i32 indent);

        /**
         * @brief Print the JSON value.
         * 
         * @param value The JSON boolean value.
         * @param os The output stream.
         * @param indent The indentation.
         */
        static void print_value(const b8& value, std::ostream& os, i32 indent);

        /**
         * @brief Print the JSON integer.
         * 
         * @param value The JSON integer value.
         * @param os The output stream.
         * @param indent The indentation.
         */
        static void print_value(const i32& value, std::ostream& os, i32 indent);

        /**
         * @brief Print the JSON floating value.
         * 
         * @param value The JSON floating value.
         * @param os The output stream.
         * @param indent The indentation.
         */
        static void print_value(const f64& value, std::ostream& os, i32 indent);

        /**
         * @brief Print the JSON value.
         * 
         * @param value The JSON string value.
         * @param os The output stream.
         * @param indent The indentation.
         */
        static void print_value(const std::string& value, std::ostream& os, i32 indent);
    
        /**
         * @brief Print the JSON value.
         * 
         * @param value The JSON array value.
         * @param os The output stream.
         * @param indent The indentation.
         */
        static void print_value(const Array& array, std::ostream& os, i32 indent);
    
        /**
         * @brief Print the JSON value.
         * 
         * @param value The JSON object value.
         * @param os The output stream.
         * @param indent The indentation.
         */
        static void print_value(const Object& object, std::ostream& os, i32 indent);
    };
}

#endif // _JOJ_JSON_VALUE_H