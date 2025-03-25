#ifndef _JOJ_JSON_VALUE_H
#define _JOJ_JSON_VALUE_H

#include "joj/core/defines.h"

#include <string>
#include <unordered_map>
#include <vector>
#include <variant>

namespace joj
{
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
        using Object = std::unordered_map<std::string, JsonValue>;
        using Array = std::vector<JsonValue>;
        using Value = std::variant<std::nullptr_t, b8, f64, std::string, Object, Array>;
    
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

    private:
        /** @brief The JSON value. */
        Value m_value;
    };
}

#endif // _JOJ_JSON_VALUE_H