#include "jscript/parser.h"

#include <iostream>

joj::Parser::Parser(Lexer& lexer)
    : m_lexer{ lexer }, m_symtable(nullptr), m_lookahead(nullptr)
{
}

joj::Parser::~Parser()
{
}

void joj::Parser::start()
{
    program();
}

void joj::Parser::program()
{
    if (!match(TokenType::FUNC_DECLARATION))
        std::cout << "Expected \'" << "func" << "\' in line " << m_lexer.get_line() << ".\n";

    if (!match(TokenType::MAIN_FUNC))
        std::cout << "Expected \'" << "main" << "\' in line " << m_lexer.get_line() << ".\n";

    if (!match(TokenType::OPEN_PAR))
        std::cout << "Expected \'" << "(" << "\' in line " << m_lexer.get_line() << ".\n";

    if (!match(TokenType::CLOSE_PAR))
        std::cout << "Expected \'" << ")" << "\' in line " << m_lexer.get_line() << ".\n";

    // program -> block
    block();
}

void joj::Parser::block()
{
    // block -> { declarations statements }
    if (!match(TokenType::OPEN_CURLY_BRAC))
        std::cout << "\'{\' expected at line " << m_lexer.get_line() << ".\n";
    else
        std::cout << "{\n";

    // New symbol table for block (new scope)
    SymbolTable* saved = m_symtable;
    m_symtable = new SymbolTable{ m_symtable };

    declarations();
    statements();

    if (!match(TokenType::CLOSE_CURLY_BRAC))
        std::cout << "\'}\' expected at line " << m_lexer.get_line() << ".\n";
    else
        std::cout << "}\n";

    // Return to previous block (inner scope)
    delete m_symtable;
    m_symtable = saved;
}

void joj::Parser::declarations()
{
    // declarations -> declaration declarations
    //               | empty
    //
    // declaration  -> VAR id : type;
    //               | VAR id;

    while (m_lookahead->type == TokenType::VAR_DECLARATION)
    {
        std::string var_decl{ m_lookahead->lexeme };
        match(TokenType::VAR_DECLARATION);

        std::string var_name{ m_lookahead->lexeme };
        match(TokenType::ID);

        // LET x : TYPE;
        if (match(TokenType::COLON))
        {
            TokenType tag{ string_to_tokentype(m_lookahead->lexeme) };
            if (!match(TokenType::INT) || !match(TokenType::FLOAT)   ||
                !match(TokenType::CHAR) || !match(TokenType::STRING) ||
                !match(TokenType::BOOLEAN))
                std::cout << "[Syntax Error]: Unknown type \'" << m_lookahead->lexeme << "\'.\n";

            Token t{ tag, var_name };

            // Insert new variable in symbol table
            if (!m_symtable->insert(var_name, t))
            {
                // Failed if variable already in table
                std::cout << "\nVariable \"" << var_name << "\" already defined.\n";
            }

            // LET x : TYPE = VALUE;
            if (match(TokenType::ASSIGN))
            {
                std::string value = m_lookahead->lexeme;
                tag = m_lookahead->type;
                if (!match(tag))
                    std::cout << "[Syntax Error]: Unknown type \'" << m_lookahead->lexeme << "\'.\n";

                if (match(TokenType::SEMICOLON))
                {
                    std::cout << "--> [NEW VARIABLE]: " << var_name << " : " << tokentype_to_string(tag)
                        << " = " << value << " .\n";
                }
                else
                {
                    std::cout << "Found \"" << m_lookahead->lexeme
                        << "\" instead of ';' in line " << m_lexer.get_line() << ".\n";
                }
            }
            // LET x : TYPE;
            else
            {
                if (!match(TokenType::SEMICOLON))
                {
                    std::cout << "Found \"" << m_lookahead->lexeme
                        << "\" instead of ';' in line " << m_lexer.get_line() << ".\n";
                }
            }

        }
        // LET x;
        else if (match(TokenType::SEMICOLON))
        {
            Token t{ TokenType::UNKNOWN, var_name };

            // Insert new variable in symbol table
            if (!m_symtable->insert(var_name, t))
            {
                // Failed if variable already in table
                std::cout << "\nVariable \"" << var_name << "\" already defined.\n";
            }
        }
        else
        {
            std::cout << "Invalid variable declaration. Found \"" << m_lookahead->lexeme
                << "\". Expected ':' or ';'.\n";
        }
    }
}

void joj::Parser::declaration()
{

}

void joj::Parser::statements()
{
    // statements -> statement statements
    //             | empty
    //
    // statement  -> block
    //             | fact;

    while (true)
    {
        switch (m_lookahead->type)
        {
            // statement -> block
        case TokenType::OPEN_CURLY_BRAC:
            block();
            break;
            // statement -> fact;
        case TokenType::ID:
            fact();
            if (!match(TokenType::SEMICOLON))
            {
                std::cout << "Found \"" << m_lookahead->lexeme
                    << "\" instead of ';' in line " << m_lexer.get_line() << ".\n";
            }
            break;
        case TokenType::VAR_DECLARATION:
            declarations();
            break;
            // statements -> empty
        default: return;
        }
    }
}

void joj::Parser::statement()
{

}

void joj::Parser::expr()
{
    // expr -> term add
    term();

    while (true)
    {
        // add -> + term { print(+) } add
        if (m_lookahead->type == TokenType::ADD_SIGN)
        {
            match(TokenType::ADD_SIGN);
            std::cout << '+';
            term();
        }
        // add -> - term { print(-) } add
        else if (m_lookahead->type == TokenType::SUB_SIGN)
        {
            match(TokenType::SUB_SIGN);
            std::cout << '-';
            term();
        }
        // add -> empty
        else return;
    }

}

void joj::Parser::term()
{
    // term -> fact mult
    fact();

    while (true)
    {
        // mult -> * fact { print(*) }
        if (m_lookahead->type == TokenType::MUL_SIGN)
        {
            match(TokenType::MUL_SIGN);
            std::cout << '*';
            fact();
        }
        // mult -> / fact { print(/) }
        else if (m_lookahead->type == TokenType::DIV_SIGN)
        {
            match(TokenType::DIV_SIGN);
            std::cout << '/';
            fact();
        }
        // mult -> empty
        else return;
    }
}


void joj::Parser::fact()
{
    // fact -> id
    if (m_lookahead->type == TokenType::ID)
    {
        // Look for variable in symbol table
        Token* t = m_symtable->find(m_lookahead->lexeme);
        if (t)
        {
            std::cout << t->lexeme << ':' << tokentype_to_string(t->type) << ";\n";
            match(TokenType::ID);
        }
        else
        {
            std::cout << "Undefined variable \"" << m_lookahead->lexeme << "\".\n";
            m_lookahead = m_lexer.scan();
        }
    }
    else
    {
        std::cout << "Invalid symbol \"" << m_lookahead->lexeme << "\" in expression.\n";
    }
}

// void joj::Parser::fact()
// {
//     // fact -> (expr)
//     if (m_lookahead->type == TokenType::OPEN_PAR)
//     {
//         match(TokenType::OPEN_PAR);
//         std::cout << '(';
//         expr();
//         if (!match(TokenType::CLOSE_PAR))
//             std::cout << "\')\' expected at line " << m_lexer.get_line() << ".\n";

//         std::cout << ')';
//     }
//     // fact -> num { print(num.value) };
//     else if (m_lookahead->type == TokenType::INT || m_lookahead->type == TokenType::FLOAT)
//     {
//         std::cout << '[' << m_lookahead->lexeme << "]";
//         match(m_lookahead->type);
//     }
//     // fact -> id { print(id.name) }
//     else if (m_lookahead->type == TokenType::ID)
//     {
//         std::cout << '[' << m_lookahead->lexeme << "]";
//         match(TokenType::ID);
//     }
//     // Syntax error
//     else
//     {
//         std::stringstream ss;
//         ss << "Invalid unknown symbol \'" << m_lookahead->lexeme << "\'.\n"; 
//         std::cout << ss.str();
//     }
// }

b8 joj::Parser::match(TokenType type)
{
    if (m_lookahead->type == type)
    {
        m_lookahead = m_lexer.scan();
        return true;
    }

    return false;
}