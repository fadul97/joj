#include "joj/jscript/parser.h"

#include <iostream>

joj::Parser::Parser(Lexer& lexer)
    : m_lexer{ lexer }, m_symtable(nullptr), m_lookahead(nullptr), m_errors(0)
{
}

joj::Parser::~Parser()
{
}

void joj::Parser::start()
{
    m_lexer.start();
    m_lookahead = m_lexer.scan();
    declaration();
}

void joj::Parser::declaration()
{
    // var y : dec = 20.05;
    /*
     * KEYWORD_VAR variable_name KEYWORD_COLON type KEYWORD_ASSIGN expression KEYWORD_SEMICOLON
     *
     * type = KEYWORD_BOOL
     *      | KEYWORD_CHAR
     *      | KEYWORD_INT
     *      | KEYWORD_FLOAT
     *      | KEYWORD_STRING
     * 
     * variable_name = any
     * 
     * expression = NUMBER
     *            | any (?)
     */

    // It will always work because while loop will break otherwise
    match(TokenType::VAR_DECLARATION);

    std::string var_name(m_lookahead->lexeme);
    if (!match(TokenType::ID))
    {
        ++m_errors;
        std::cerr << "Error: expected variable name after 'var' keyword\n";
        return;
    }

    if (!match(TokenType::COLON))
    {
        ++m_errors;
        std::cerr << "Error: expected ':' after variable name\n";
        return;
    }

    const TokenType type = m_lookahead->type;

    if (!match(TokenType::BOOLEAN) && !match(TokenType::CHAR)  &&
        !match(TokenType::INT)     && !match(TokenType::FLOAT) &&
        !match(TokenType::STRING))
    {
        ++m_errors;
        std::cerr << "Error: expected type after ':'\n";
        return;
    }

    if (!match(TokenType::ASSIGN))
    {
        ++m_errors;
        std::cerr << "Error: expected '=' after type\n";
        return;
    }

    switch (type)
    {
    case TokenType::BOOLEAN:
        if (!match(TokenType::VALUE_BOOLEAN))
        {
            ++m_errors;
            std::cerr << "Error: expected boolean value after '='\n";
            return;
        }
        break;
    case TokenType::CHAR:
        if (!match(TokenType::VALUE_CHAR))
        {
            ++m_errors;
            std::cerr << "Error: expected char value after '='\n";
            return;
        }
        break;
    case TokenType::INT:
        if (!match(TokenType::VALUE_INT))
        {
            ++m_errors;
            std::cerr << "Error: expected integer value after '='\n";
            return;
        }
        break;
    case TokenType::FLOAT:
        if (!match(TokenType::VALUE_FLOAT))
        {
            ++m_errors;
            std::cerr << "Error: expected float value after '='\n";
            return;
        }
        break;
    case TokenType::STRING:
        if (!match(TokenType::VALUE_STRING))
        {
            ++m_errors;
            std::cerr << "Error: expected string value after '='\n";
            return;
        }
        break;        
    default:
        break;
    }

    if (!match(TokenType::SEMICOLON))
    {
        ++m_errors;
        std::cerr << "Error: expected ';' after variable value\n";
        return;
    }
}

b8 joj::Parser::match(const TokenType type)
{
    if (m_lookahead->type == type)
    {
        m_lookahead = m_lexer.scan();
        return true;
    }

    return false;
}

u32 joj::Parser::get_error_count() const
{
    return m_errors + m_lexer.get_error_count();
}