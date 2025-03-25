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
    case '{': return { JsonTokenType::LeftBrace, "{" };
    case '}': return { JsonTokenType::RightBrace, "}" };
    case '[': return { JsonTokenType::LeftBracket, "[" };
    case ']': return { JsonTokenType::RightBracket, "]" };
    case ':': return { JsonTokenType::Colon, ":" };
    case ',': return { JsonTokenType::Comma, "," };
    case '"': return parse_string();
    case 't': return parse_keyword("true", JsonTokenType::Boolean);
    case 'f': return parse_keyword("false", JsonTokenType::Boolean);
    case 'n': return parse_keyword("null", JsonTokenType::Null);
    default:
    {
        if (isdigit(c) || c == '-')
            return parse_number();

        return { JsonTokenType::Error, std::string(1, c) };
    }
        break;
    }
}

joj::JsonToken joj::JsonLexer::parse_string()
{
    std::string value;
    while (peek() != '"' && peek() != '\0')
        value += advance();

    if (peek() == '\0')
        return { JsonTokenType::Error, value };

    advance(); // Skip the closing quote

    return { JsonTokenType::String, value };
}

joj::JsonToken joj::JsonLexer::parse_number()
{
    std::string value;
    while (isdigit(peek()) || peek() == '.' || peek() == '-')
        value += advance();

    return { JsonTokenType::Number, value };
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