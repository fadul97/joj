#include "joj/utils/json_lexer.h"

joj::JsonLexer::JsonLexer(std::string_view json)
    : m_input(json), m_position(0), m_error_count(0)
{
}

joj::JsonLexer::~JsonLexer()
{
}

joj::JsonToken joj::JsonLexer::next_token()
{
    skip_whitespace();

    char c = advance();
    switch (c)
    {
    case '{':  return { JsonTokenType::LeftBrace, "{" };
    case '}':  return { JsonTokenType::RightBrace, "}" };
    case '[':  return { JsonTokenType::LeftBracket, "[" };
    case ']':  return { JsonTokenType::RightBracket, "]" };
    case ':':  return { JsonTokenType::Colon, ":" };
    case ',':  return { JsonTokenType::Comma, "," };
    case '"':  return parse_string();
    case 't':  return parse_keyword(c, "true", JsonTokenType::Boolean);
    case 'f':  return parse_keyword(c, "false", JsonTokenType::Boolean);
    case 'n':  return parse_keyword(c, "null", JsonTokenType::Null);
    case '\0': return { JsonTokenType::EndOfFile, "" };
    default:
    {
        if (isdigit(c) || c == '-')
            return parse_number(c);

        return { JsonTokenType::Error, std::string(1, c) };
    }
        break;
    }
}

joj::JsonToken joj::JsonLexer::parse_string()
{
    std::string result;
    
    while (m_position < m_input.size())
    {
        char c = advance();
        if (c == '"')
            break;  // End of string

        // Escape sequence
        if (c == '\\')
        { 
            char escaped = advance();
            switch (escaped)
            {
                case '"': result += '"'; break;
                case '\\': result += '\\'; break;
                case '/': result += '/'; break;
                case 'b': result += '\b'; break;
                case 'f': result += '\f'; break;
                case 'n': result += '\n'; break;
                case 'r': result += '\r'; break;
                case 't': result += '\t'; break;
                default:
                    ++m_error_count;
                    return { JsonTokenType::Error, "Invalid escape sequence" };
            }
        }
        else
        {
            result += c;
        }
    }

    return { JsonTokenType::String, result };
}

joj::JsonToken joj::JsonLexer::parse_number(const char c)
{
    std::string result;
    b8 is_float = false;
    b8 is_neg = false;
    
    result += c;

    // Check if it's a negative number
    if (peek() == '-')
        is_neg = true;

    while (isdigit(peek()) || peek() == '.')
    {
        if (peek() == '.')
        {
            // Check if there is another dot
            if (is_float)
                break;

            is_float = true;
        }

        result += advance();
    }

    // Check for Scientific notation (e or E)
    if (peek() == 'e' || peek() == 'E')
    {
        // Scientific notation always indicates a float number
        is_float = true;

        // Consume 'e' or 'E'
        result += advance();

        // Check if there is a sign after 'e'/'E' and consume it
        if (peek() == '+' || peek() == '-')
            result += advance();

        // Now, read the digits of the exponent
        while (isdigit(peek()))
            result += advance();
    }

    if (is_float)
        return { JsonTokenType::Float, result };

    return { JsonTokenType::Integer, result };
}

joj::JsonToken joj::JsonLexer::parse_keyword(char c, const std::string& keyword, const JsonTokenType type)
{
    for (u32 i = 0; i < keyword.size(); ++i)
    {
        // Check if the keyword matches
        if (keyword[i] != c)
        {
            ++m_error_count;
            return { JsonTokenType::Error, keyword };
        }

        // Advance to the next character
        if (peek() != ',')
            c = advance();
    }

    return { type, keyword };
}

char joj::JsonLexer::peek() const
{
    return m_position < m_input.size() ? m_input[m_position] : '\0';
}

char joj::JsonLexer::advance()
{
    return m_position < m_input.size() ? m_input[m_position++] : '\0';
}

void joj::JsonLexer::skip_whitespace()
{
    while (isspace(peek()))
        advance();
}

u32 joj::JsonLexer::get_error_count()
{
    return m_error_count;
}