#ifndef _JOJ_JSON_LEXER_H
#define _JOJ_JSON_LEXER_H

#include "joj/core/defines.h"

#include <string_view>
#include "json_token.h"

namespace joj
{
    /**
     * @brief A class that represents a JSON lexer.
     * The lexer takes a JSON string_view and returns tokens.
     */
    class JOJ_API JsonLexer
    {
    public:
        /**
         * @brief Construct a new Json Lexer object.
         * 
         * @param json The JSON string_view.
         */
        JsonLexer(std::string_view json);

        /**
         * @brief Destroy the Json Lexer object.
         */
        ~JsonLexer();

        /**
         * @brief Get the next token.
         * 
         * @return The next token.
         */
        JsonToken next_token();

        /**
         * @brief Parse a string.
         * 
         * @return JsonToken The string token.
         */
        JsonToken parse_string();

        /**
         * @brief Parse a number.
         * 
         * @param c The current character.
         * @return JsonToken The number token.
         */
        JsonToken parse_number(const char c);

        /**
         * @brief Parse a keyword value.
         * 
         * @return JsonToken The keyword token.
         */
        JsonToken parse_keyword(char c, const std::string& keyword, const JsonTokenType type);

        /**
         * @brief Peek at the next character.
         * 
         * @return The next character.
         */
        char peek() const;

        /**
         * @brief Advance the position.
         * 
         * @return The next character.
         */
        char advance();

        /**
         * @brief Skip whitespace characters.
         */
        void skip_whitespace();

    private:
        std::string_view m_input;
        u32 m_position;
    };
}

#endif // _JOJ_JSON_LEXER_H