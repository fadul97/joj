#include "jscript/lexer.h"

#include "jscript/keywords.h"
#include "core/logger.h"
#include <cctype>
#include <sstream>

joj::Lexer::Lexer(const std::string& source)
    : m_source(source), m_index(0), m_lineno(1),
    m_peek(' '), m_errors(0)
{
    m_token = Token();

    m_token_table[KEYWORD_LET]              = Token{ TokenType::VAR_DECLARATION,  KEYWORD_LET };
    m_token_table[KEYWORD_COLON]            = Token{ TokenType::COLON,            KEYWORD_COLON };
    m_token_table[KEYWORD_INT]              = Token{ TokenType::INT,              KEYWORD_INT };
    m_token_table[KEYWORD_FLOAT]            = Token{ TokenType::FLOAT,            KEYWORD_FLOAT };
    m_token_table[KEYWORD_CHAR]             = Token{ TokenType::CHAR,             KEYWORD_CHAR };
    m_token_table[KEYWORD_BOOL]             = Token{ TokenType::BOOLEAN,          KEYWORD_BOOL };
    m_token_table[KEYWORD_STRING]           = Token{ TokenType::STRING,           KEYWORD_STRING };
    m_token_table[KEYWORD_ASSIGN]           = Token{ TokenType::ASSIGN,           KEYWORD_ASSIGN };
    m_token_table[KEYWORD_SEMICOLON]        = Token{ TokenType::SEMICOLON,        KEYWORD_SEMICOLON };
    m_token_table[KEYWORD_FUNC]             = Token{ TokenType::FUNC_DECLARATION, KEYWORD_FUNC };
    m_token_table[KEYWORD_MAIN_FUNC]        = Token{ TokenType::MAIN_FUNC,        KEYWORD_MAIN_FUNC };
    m_token_table[KEYWORD_IF]               = Token{ TokenType::IF,               KEYWORD_IF };
    m_token_table[KEYWORD_ELSE]             = Token{ TokenType::ELSE,             KEYWORD_ELSE };
    m_token_table[KEYWORD_ELSEIF]           = Token{ TokenType::ELSE_IF,          KEYWORD_ELSEIF };
    m_token_table[KEYWORD_MATCH]            = Token{ TokenType::MATCH,            KEYWORD_MATCH };
    m_token_table[KEYWORD_FOR]              = Token{ TokenType::FOR,              KEYWORD_FOR };
    m_token_table[KEYWORD_IN]               = Token{ TokenType::IN,               KEYWORD_IN };
    m_token_table[KEYWORD_WHILE]            = Token{ TokenType::WHILE,            KEYWORD_WHILE };
    m_token_table[KEYWORD_LOOP]             = Token{ TokenType::LOOP,             KEYWORD_LOOP };
    m_token_table[KEYWORD_BREAK]            = Token{ TokenType::BREAK,            KEYWORD_BREAK };
    m_token_table[KEYWORD_TRUE]             = Token{ TokenType::TRUE,             KEYWORD_TRUE };
    m_token_table[KEYWORD_FALSE]            = Token{ TokenType::FALSE,            KEYWORD_FALSE };
    m_token_table[KEYWORD_EQUAL]            = Token{ TokenType::EQUALS,           KEYWORD_EQUAL };
    m_token_table[KEYWORD_NOT_EQUAL]        = Token{ TokenType::NOT_EQUAL,        KEYWORD_NOT_EQUAL };
    m_token_table[KEYWORD_GREATER_THAN]     = Token{ TokenType::GREATER_THAN,     KEYWORD_GREATER_THAN };
    m_token_table[KEYWORD_LESS_THAN]        = Token{ TokenType::AND,              KEYWORD_LESS_THAN };
    m_token_table[KEYWORD_AND]              = Token{ TokenType::VAR_DECLARATION,  KEYWORD_AND };
    m_token_table[KEYWORD_OR]               = Token{ TokenType::OR,               KEYWORD_OR };
    m_token_table[KEYWORD_ADD_SIGN]         = Token{ TokenType::ADD_SIGN,         KEYWORD_ADD_SIGN };
    m_token_table[KEYWORD_SUB_SIGN]         = Token{ TokenType::SUB_SIGN,         KEYWORD_SUB_SIGN };
    m_token_table[KEYWORD_DIV_SIGN]         = Token{ TokenType::DIV_SIGN,         KEYWORD_DIV_SIGN };
    m_token_table[KEYWORD_MUL_SIGN]         = Token{ TokenType::MUL_SIGN,         KEYWORD_MUL_SIGN };
    m_token_table[KEYWORD_OPEN_PAR]         = Token{ TokenType::OPEN_PAR,         KEYWORD_OPEN_PAR };
    m_token_table[KEYWORD_CLOSE_PAR]        = Token{ TokenType::CLOSE_PAR,        KEYWORD_CLOSE_PAR };
    m_token_table[KEYWORD_OPEN_BRAC]        = Token{ TokenType::OPEN_BRAC,        KEYWORD_OPEN_BRAC };
    m_token_table[KEYWORD_CLOSE_BRAC]       = Token{ TokenType::CLOSE_BRAC,       KEYWORD_CLOSE_BRAC };
    m_token_table[KEYWORD_OPEN_CURLY_BRAC]  = Token{ TokenType::OPEN_CURLY_BRAC,  KEYWORD_OPEN_CURLY_BRAC };
    m_token_table[KEYWORD_CLOSE_CURLY_BRAC] = Token{ TokenType::CLOSE_CURLY_BRAC, KEYWORD_CLOSE_CURLY_BRAC };
    m_token_table[KEYWORD_COUNTER_BAR]      = Token{ TokenType::COUNTER_BAR,      KEYWORD_COUNTER_BAR };
    m_token_table[KEYWORD_PIPE]             = Token{ TokenType::PIPE,             KEYWORD_PIPE };

}

joj::Lexer::~Lexer()
{
}

void joj::Lexer::start()
{
    m_fin.open(m_source);

    if (!m_fin.is_open())
    {
        JERROR(ErrorCode::FAILED, "Failed to open file '%s'\n."
            "Check file name and location. Check if it is read - protected.",
            m_source.c_str());
    }
}

joj::Token* joj::Lexer::scan()
{
    // Ignore white spaces and new lines
    while (isspace(m_peek))
    {
        if (m_peek == '\n')
            m_lineno++;

        m_peek = static_cast<char>(m_fin.get());
    }

    // TODO: Ignore comments

    // Return int or float numbers
    if (isdigit(m_peek))
    {
        b8 dot = false;
        std::stringstream ss;

        do
        {
            if (m_peek == '.')
            {
                if (!dot)
                    dot = true;
                else
                {
                    JERROR(ErrorCode::FAILED, "Unexpected '.' in line %d.", m_lineno);
                    ++m_errors;
                }
            }

            ss << m_peek;
            m_peek = static_cast<char>(m_fin.get());

        } while (isdigit(m_peek) || m_peek == '.');

        if (dot)
        {
            Token t{ TokenType::NUMBER_FLOAT, ss.str() };

            m_token = t;
            return &m_token;
        }
        else
        {
            Token t{ TokenType::NUMBER_INT, ss.str() };

            m_token = t;
            return &m_token;
        }
    }

    // Return keywords or ids
    if (isalpha(m_peek) || m_peek == '_')
    {
        std::stringstream ss;

        do
        {
            ss << m_peek;
            m_peek = static_cast<char>(m_fin.get());

            // TODO: Refactor condition
        } while ((!isspace(m_peek) || isalpha(m_peek) || m_peek == '_' || isdigit(m_peek))
            && m_peek != ':' && m_peek != '=' && m_peek != ';' && m_peek != '-' && m_peek != '?'
            && m_peek != '/' && m_peek != '\\' && m_peek != '|' && m_peek != '`' && m_peek != '\''
            && m_peek != '"' && m_peek != '(' && m_peek != ')' && m_peek != '[' && m_peek != ']'
            && m_peek != '{' && m_peek != '}' && m_peek != '+' && m_peek != '*' && m_peek != '.'
            && m_peek != '!' && m_peek != '@' && m_peek != '#' && m_peek != '$' && m_peek != '%'
            && m_peek != '^' && m_peek != '&' && m_peek != '~');

        std::string s = ss.str();
        auto pos = m_token_table.find(s);

        // If already on table
        if (pos != m_token_table.end())
        {
            m_token = pos->second;
            // std::cout << token.to_string() << "\n";

            return &m_token;
        }

        Token t{ TokenType::ID, s };
        m_token_table[s] = t;
        // std::cout << t.to_string() << "\n";

        m_token = t;
        return &m_token;
    }

    // Return char
    if (m_peek == '\'')
    {
        m_peek = static_cast<char>(m_fin.get());
        // std::cout << "<LETRA, " << peek << ">\n";

        m_token = Token{ TokenType::CHAR, std::string{m_peek} };
        // std::cout << token.to_string() << "\n";

        // Ignore second '
        m_peek = static_cast<char>(m_fin.get());

        // Get next symbol
        m_peek = static_cast<char>(m_fin.get());

        // Return token
        return &m_token;
    }

    // Return string
    if (m_peek == '"')
    {
        std::stringstream ss;

        // Ignore '"'
        m_peek = static_cast<char>(m_fin.get());

        do
        {
            if (m_peek == '\\')
            {
                ss << m_peek;
                m_peek = static_cast<char>(m_fin.get());

                // TODO: Fix '\' when '\\"' and other occurrences
                if (m_peek == '"' || m_peek == '\\')
                {
                    ss << m_peek;
                    m_peek = static_cast<char>(m_fin.get());
                    continue;
                }
                else
                {
                    // Raw string literal
                    JERROR(ErrorCode::FAILED, "Expected '\"' or '\\'' after first '\\'' in line %d", m_lineno);
                    ++m_errors;
                }
            }

            ss << m_peek;
            m_peek = static_cast<char>(m_fin.get());

        } while (m_peek != '"');

        // Ignore second '"'
        m_peek = static_cast<char>(m_fin.get());

        m_token = Token{ TokenType::STRING, ss.str() };
        // std::cout << token.to_string() << ">\n";

        return &m_token;
    }

    // TODO: Return 2 chars operators: ==, !=, >=, <=, && and ||
    if (m_peek == '=')
    {
        char nextChar = static_cast<char>(m_fin.peek());
        if (nextChar == '=')
        {
            m_token = Token{ TokenType::EQUALS, "==" };
            m_peek = ' ';
            m_fin.get();
            return &m_token;
        }
    }
    else if (m_peek == '!')
    {
        char nextChar = static_cast<char>(m_fin.peek());
        if (nextChar == '=')
        {
            m_token = Token{ TokenType::NOT_EQUAL, "!=" };
            m_peek = ' ';
            m_fin.get();
            return &m_token;
        }
    }
    else if (m_peek == '>')
    {
        char nextChar = static_cast<char>(m_fin.peek());
        if (nextChar == '=')
        {
            m_token = Token{ TokenType::GREATER_THAN, ">=" };
            m_peek = ' ';
            m_fin.get();
            return &m_token;
        }
    }
    else if (m_peek == '<')
    {
        char nextChar = static_cast<char>(m_fin.peek());
        if (nextChar == '=')
        {
            m_token = Token{ TokenType::LESS_THAN, "<=" };
            m_peek = ' ';
            m_fin.get();
            return &m_token;
        }
    }
    else if (m_peek == '&')
    {
        char nextChar = static_cast<char>(m_fin.peek());
        if (nextChar == '&')
        {
            m_token = Token{ TokenType::AND };
            m_peek = ' ';
            m_fin.get();
            return &m_token;
        }
    }
    else if (m_peek == '|')
    {
        char nextChar = static_cast<char>(m_fin.peek());
        if (nextChar == '|')
        {
            m_token = Token{ TokenType::OR };
            m_peek = ' ';
            m_fin.get();
            return &m_token;
        }
    }

    if (m_peek == -1)
    {
        m_token = Token{ TokenType::END_OF_FILE };
        return &m_token;
    }

    // Return isolated operators: :, =, ;, (, ), [, ], {, }, +, -, /, !, \, |
    switch (m_peek) {
    case ':':
        m_token = Token{ TokenType::COLON, ":"};
        m_peek = static_cast<char>(m_fin.get());
        return &m_token;
    case '=':
        m_token = Token{ TokenType::ASSIGN, "=" };
        m_peek = static_cast<char>(m_fin.get());
        return &m_token;
    case ';':
        m_token = Token{ TokenType::SEMICOLON, ";" };
        m_peek = static_cast<char>(m_fin.get());
        return &m_token;
    case '(':
        m_token = Token{ TokenType::OPEN_PAR, "(" };
        m_peek = static_cast<char>(m_fin.get());
        return &m_token;
    case ')':
        m_token = Token{ TokenType::CLOSE_PAR, ")" };
        m_peek = static_cast<char>(m_fin.get());
        return &m_token;
    case '[':
        m_token = Token{ TokenType::OPEN_BRAC, "[" };
        m_peek = static_cast<char>(m_fin.get());
        return &m_token;
    case ']':
        m_token = Token{ TokenType::CLOSE_BRAC, "]" };
        m_peek = static_cast<char>(m_fin.get());
        return &m_token;
    case '{':
        m_token = Token{ TokenType::OPEN_CURLY_BRAC, "{" };
        m_peek = static_cast<char>(m_fin.get());
        return &m_token;
    case '}':
        m_token = Token{ TokenType::CLOSE_CURLY_BRAC, "}" };
        m_peek = static_cast<char>(m_fin.get());
        return &m_token;
    case '+':
        m_token = Token{ TokenType::ADD_SIGN, "+" };
        m_peek = static_cast<char>(m_fin.get());
        return &m_token;
    case '-':
        m_token = Token{ TokenType::SUB_SIGN, "-" };
        m_peek = static_cast<char>(m_fin.get());
        return &m_token;
    case '/':
        m_token = Token{ TokenType::DIV_SIGN, "/" };
        m_peek = static_cast<char>(m_fin.get());
        return &m_token;
    case '*':
        m_token = Token{ TokenType::MUL_SIGN, "*" };
        m_peek = static_cast<char>(m_fin.get());
        return &m_token;
    case '!':
        m_token = Token{ TokenType::EXCLAMATION, "!" };
        m_peek = static_cast<char>(m_fin.get());
        return &m_token;
    case '\\':
        m_token = Token{ TokenType::COUNTER_BAR, "'\\'" };
        m_peek = static_cast<char>(m_fin.get());
        return &m_token;
    case '|':
        m_token = Token{ TokenType::PIPE, "|" };
        m_peek = static_cast<char>(m_fin.get());
        return &m_token;
    default:
        m_token = Token{ TokenType::UNKNOWN, std::string(1, m_peek) };
        break;
    }

    m_peek = ' '; // Reseta m_peek para continuar a leitura

    return &m_token;
}

joj::Vector<joj::Token> joj::Lexer::tokenize()
{
    return Vector<Token>();
}