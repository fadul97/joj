#include "joj/jscript/parser.h"

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
    m_lexer.start();
    m_lookahead = m_lexer.scan();
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
            TokenType ttype{ string_to_tokentype(m_lookahead->lexeme) };
            if (match(TokenType::INT) || match(TokenType::FLOAT) ||
                match(TokenType::CHAR) || match(TokenType::STRING) ||
                match(TokenType::BOOLEAN))
            {
                Token t{ ttype, var_name };

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
                    ttype = m_lookahead->type;
                    if (!match(ttype))
                        std::cout << "[Syntax Error]: Unknown type \'" << m_lookahead->lexeme << "\'.\n";

                    if (match(TokenType::SEMICOLON))
                    {
                        std::cout << "--> [NEW VARIABLE]: " << var_name << " : " << tokentype_to_string(ttype)
                            << " = " << value << ".\n";
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
                    if (match(TokenType::SEMICOLON))
                    {
                        std::cout << "--> [NEW VARIABLE]: " << var_name << " : " << tokentype_to_string(ttype)
                            << ".\n";
                    }
                    else
                    {
                        std::cout << "Found \"" << m_lookahead->lexeme
                            << "\" instead of ';' in line " << m_lexer.get_line() << ".\n";
                    }
                }
            }
            else
            {
                std::cout << "[Syntax Error]: Unknown type \'" << m_lookahead->lexeme << "\'.\n";
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

            std::cout << "--> [NEW VARIABLE]: " << var_name << " : " << tokentype_to_string(t.type)
                << ".\n";
        }
        // LET x = 10;
        else if (match(TokenType::ASSIGN))
        {
            std::string value = m_lookahead->lexeme;
            Token t{ TokenType::ASSIGN, value };
            if (match(TokenType::VALUE_INT) || match(TokenType::VALUE_FLOAT) ||
                match(TokenType::VALUE_CHAR) || match(TokenType::VALUE_STRING) ||
                match(TokenType::VALUE_BOOLEAN))
            {
                if (match(TokenType::SEMICOLON))
                {
                    std::cout << "--> [NEW VARIABLE]: " << var_name << " : "
                        << " = " << value << ".\n";
                }
                else
                {
                    std::cout << "Found \"" << m_lookahead->lexeme
                        << "\" instead of ';' in line " << m_lexer.get_line() << ".\n";
                }
            }
            else
            {
                std::cout << "[Syntax Error]: Expected value, found unknown lexeme \'" << m_lookahead->lexeme << "\'.\n";
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

joj::Statement* joj::Parser::statements()
{
    // statements -> statement statements
    //             | empty

    Statement* seq = nullptr;

    switch (m_lookahead->type)
    {
    case TokenType::ID:
    case TokenType::IF:
    case TokenType::WHILE:
    case TokenType::LOOP:
    case TokenType::OPEN_CURLY_BRAC:
    {
        Statement* st = statement();
        Statement* sts = statements();
        seq = new Seq(st, sts);
    }
        break;
    default:
        break;
    }

    return seq;
}

joj::Statement* joj::Parser::statement()
{
    // statement  -> local = bool;
    //             | if (bool) statement
    //             | while (bool) statement
    //             | loop statement
    //             | block

    Statement* stmt = nullptr;

    switch (m_lookahead->type)
    {
    // Statement -> local = bool
    case TokenType::ID:
    {
        Expression* left = local();
        if (!match(TokenType::ASSIGN))
        {
            std::cout << "Expected '=' instead of '" << m_lookahead->lexeme << "\n";
            return stmt;
        }

        Expression* right = Bool();
        stmt = new Assignment(left, right);
        if (!match(TokenType::SEMICOLON))
        {
            std::cout << "Expected ';' instead of '" << m_lookahead->lexeme << "\n";
            return stmt;
        }
    }
        break;
    default:
        break;
    }

    return stmt;
}

joj::Expression* joj::Parser::local()
{
    // local -> local[bool]
    //        | id

    Expression* expr = nullptr;

    switch (m_lookahead->type)
    {
    case TokenType::ID:
    {
        // Find variable type in symbol table
        Token* t = m_symtable->find(m_lookahead->lexeme);
        if (!t)
        {
            std::cout << "Undeclared variable '" << m_lookahead->lexeme << "'.\n";
            return expr;
        }

        // Get type of expression
        ExpressionType etype = string_to_exprtype(m_lookahead->lexeme);

        // ID
        expr = new Identifier(etype, new Token(*m_lookahead));
        match(TokenType::ID);
    }
        break;
    default:
        std::cout << "Expected L-Value.\n";
        break;
    }

    return expr;
}

joj::Expression* joj::Parser::Bool()
{
    // bool -> join lor
    // lor  -> || join lor
    //       | empty
    return nullptr;
}

joj::Expression* joj::Parser::join()
{
    // join -> equality land
    // land -> && equality land
    //       | empty

    Expression* expr1 = equality();
    return nullptr;
}

joj::Expression* joj::Parser::equality()
{
    // equality -> rel eqdif
    // eqdif    -> == rel eqdif
    //           | != rel eqdif
    //           | empty
    return nullptr;
}

joj::Expression* joj::Parser::rel()
{
    // equality -> rel eqdif
    // eqdif    -> == rel eqdif
    //           | != rel eqdif
    //           | empty
    return nullptr;
}

joj::Expression* joj::Parser::ari()
{
    // ari  -> term oper
    // oper -> + term oper
    //       | - term oper
    //       | empty

    Expression* expr1 = term();

    // Function oper()
    while (true)
    {
        Token t = *m_lookahead;

        // oper -> + term oper
        if (m_lookahead->type == TokenType::ADD_SIGN)
        {
            match(TokenType::ADD_SIGN);
            Expression* expr2 = term();
            expr1 = new Arithmetic(expr1->get_type(), new Token{t}, expr1, expr2);
        }
        // oper -> - term oper
        else if (m_lookahead->type == TokenType::SUB_SIGN)
        {
            match(TokenType::SUB_SIGN);
            Expression* expr2 = term();
            expr1 = new Arithmetic(expr1->get_type(), new Token{ t }, expr1, expr2);
        }
        // oper -> empty
        else
        {
            break;
        }
    }

    return expr1;
}

joj::Expression* joj::Parser::term()
{
    // term -> unary calc
    // calc -> * unary calc
    //       | / unary calc
    //       | empty

    Expression* expr1 = unary();

    // Function calc()
    while (true)
    {
        Token t = *m_lookahead;

        // calc -> * unary calc
        if (m_lookahead->type == TokenType::MUL_SIGN)
        {
            match(TokenType::MUL_SIGN);
            Expression* expr2 = unary();
            expr1 = new Arithmetic(expr1->get_type(), new Token{t}, expr1, expr2);
        }
        // calc -> / unary calc
        else if (m_lookahead->type == TokenType::DIV_SIGN)
        {
            match(TokenType::DIV_SIGN);
            Expression* expr2 = unary();
            expr1 = new Arithmetic(expr1->get_type(), new Token{ t }, expr1, expr2);
        }
        // calc -> empty
        else
        {
            break;
        }
    }

    return nullptr;
}

joj::Expression* joj::Parser::unary()
{
    // unary -> !unary
    //        | -unary
    //        | factor

    Expression* unary_expr = nullptr;

    // unary -> !unary
    if (m_lookahead->type == TokenType::EXCLAMATION)
    {
        Token t = *m_lookahead;
        match(TokenType::EXCLAMATION);
        Expression* expr = unary();
        unary_expr = new UnaryExpr(ExpressionType::BOOL, new Token(t), expr);
    }
    // unary -> -unary
    else if (m_lookahead->type == TokenType::SUB_SIGN)
    {
        Token t = *m_lookahead;
        match(TokenType::SUB_SIGN);
        Expression* expr = unary();
        unary_expr = new UnaryExpr(expr->get_type(), new Token{t}, expr);
    }
    else
    {
        unary_expr = factor();
    }

    return unary_expr;
}

joj::Expression* joj::Parser::factor()
{
    // factor -> (bool)
    //         | local
    //         | integer
    //         | float
    //         | true
    //         | false

    Expression* expr = nullptr;

    switch (m_lookahead->type)
    {
    // factor -> (bool)
    case TokenType::OPEN_PAR:
    {
        match(TokenType::OPEN_PAR);
        expr = Bool();
        if (!match(TokenType::CLOSE_PAR))
        {
            std::cout << "Expected ')' instead of '" << m_lookahead->lexeme << "\n";
            return expr;
        }
    }
        break;
    // factor -> integer
    case TokenType::VALUE_INT:
    {
        expr = new Constant(ExpressionType::INT, new Token{ *m_lookahead });
        match(TokenType::VALUE_INT);
    }
        break;
    // factor -> float
    case TokenType::VALUE_FLOAT:
    {
        expr = new Constant(ExpressionType::FLOAT, new Token{ *m_lookahead });
        match(TokenType::VALUE_FLOAT);
    }
        break;
    // factor -> true
    case TokenType::TRUE:
    {
        expr = new Constant(ExpressionType::BOOL, new Token{ *m_lookahead });
        match(TokenType::TRUE);
    }
        break;
    // factor -> true
    case TokenType::FALSE:
    {
        expr = new Constant(ExpressionType::BOOL, new Token{ *m_lookahead });
        match(TokenType::FALSE);
    }
        break;
    default:
        std::cout << "Expected expression instead of '" << m_lookahead->lexeme << "\n";
        break;
    }

    return expr;
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