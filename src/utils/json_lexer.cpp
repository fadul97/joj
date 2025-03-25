#include "joj/utils/json_lexer.h"

joj::JsonLexer::JsonLexer(std::string_view json)
    : m_input(json)
    , m_position(0)
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
    case 't':  return parse_keyword("true", JsonTokenType::Boolean);
    case 'f':  return parse_keyword("false", JsonTokenType::Boolean);
    case 'n':  return parse_keyword("null", JsonTokenType::Null);
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

    return { JsonTokenType::Number, result };
}

joj::JsonToken joj::JsonLexer::parse_keyword(const std::string& keyword, const JsonTokenType type)
{
    for (u32 i = 0; i < keyword.size(); ++i)
    {
        if (keyword[i] != advance())
            return { JsonTokenType::Error, keyword };
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