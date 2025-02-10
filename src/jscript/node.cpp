#include "jscript/node.h"

#include "jscript/expression_type.h"
#include "jscript/node_type.h"

#include <memory>
#include <sstream>

#include <iostream>

// Node
joj::Node::Node()
    : node_type{ NodeType::UNKNOWN }
{
}

joj::Node::Node(NodeType ntype)
    : node_type{ ntype }
{
}

// Statement
joj::Statement::Statement()
    : Node{ NodeType::STATEMENT }
{
}

joj::Statement::Statement(NodeType ntype)
    : Node{ ntype }
{
}

// Expression
joj::Expression::Expression(Token* t)
    : Node{ NodeType::EXPR }, expr_type{ ExpressionType::VOID }, token{ t }
{
}

joj::Expression::Expression(NodeType ntype, ExpressionType etype, Token* t)
    : Node{ ntype }, expr_type{ etype }, token{ t }
{
}

std::string joj::Expression::get_lexeme()
{
    return token->lexeme;
}

joj::ExpressionType joj::Expression::get_type()
{
    return expr_type;
}

// Constant
joj::Constant::Constant(ExpressionType etype, Token* t)
    : Expression{ NodeType::CONSTANT, etype, t }
{
}

// Identifier
joj::Identifier::Identifier(ExpressionType etype, Token* t)
    : Expression{ NodeType::IDENTIFIER, etype, t }
{
}

// Logical
joj::Logical::Logical(Token* t, Expression* e1, Expression* e2)
    : Expression{ NodeType::LOG, ExpressionType::BOOL, t }, expr1{ e1 }, expr2{ e2 }
{
    // Check types
    if (expr1->get_type() != ExpressionType::BOOL ||
        expr2->get_type() != ExpressionType::BOOL)
    {
        std::cout << "\'" << token->lexeme << "\' used with non bool operands "
            << "(" << expr1->get_lexeme() << ":" << exprtype_to_string(expr1->get_type()) << ") ("
            << "(" << expr2->get_lexeme() << ":" << exprtype_to_string(expr2->get_type()) << ").\n";
    }
}

// Relational
joj::Relational::Relational(Token* t, Expression* e1, Expression* e2)
    : Expression{ NodeType::REL, ExpressionType::BOOL, t }, expr1{ e1 }, expr2{ e2 }
{
    // Check types
    if (expr1->get_type() != expr2->get_type())
    {
        std::cout << "\'" << token->lexeme << "\' used with different types "
            << "(" << expr1->get_lexeme() << ":" << exprtype_to_string(expr1->get_type()) << ") ("
            << "(" << expr2->get_lexeme() << ":" << exprtype_to_string(expr2->get_type()) << ").\n";
    }
}

// Arithmetic
joj::Arithmetic::Arithmetic(ExpressionType etype, Token* t, Expression* e1, Expression* e2)
    : Expression{ NodeType::ARI, etype, t }, expr1{ e1 }, expr2{ e2 }
{
    // Check types
    if (expr1->get_type() != expr2->get_type())
    {
        std::cout << "\'" << token->lexeme << "\' used with different types "
            << "(" << expr1->get_lexeme() << ":" << exprtype_to_string(expr1->get_type()) << ") ("
            << "(" << expr2->get_lexeme() << ":" << exprtype_to_string(expr2->get_type()) << ").\n";
    }
}

// UnaryExpr
joj::UnaryExpr::UnaryExpr(ExpressionType etype, Token* t, Expression* e)
    : Expression{ NodeType::UNARY, etype, t }, expr{ e }
{
    // Check types
    if (expr->get_type() != ExpressionType::BOOL)
    {
        std::cout << "\'" << token->lexeme << "\' used with non boolean "
            << "(" << expr->get_lexeme() << ":" << exprtype_to_string(expr->get_type()) << ").\n";
    }
}

// Seq
joj::Seq::Seq(Statement* s, Statement* ss)
    : Statement{ NodeType::SEQ }, stmt{ s }, stmts{ ss }
{
}

// Assignment
joj::Assignment::Assignment(Expression* i, Expression* e)
    : Statement{ NodeType::ASSIGNMENT }, id{ i }, expr{ e }
{
    // Check types
    if (id->get_type() != expr->get_type())
    {
        std::cout << "\'=\' used with different types "
            << "(" << id->get_lexeme() << ":" << exprtype_to_string(id->get_type()) << ") ("
            << "(" << expr->get_lexeme() << ":" << exprtype_to_string(expr->get_type()) << ").\n";
    }
}

// If
joj::If::If(Expression* e, Statement* s)
    : Statement{ NodeType::IF_STATEMENT }, expr{ e }, stmt{ s }
{
}

// While
joj::While::While(Expression* e, Statement* s)
    : Statement{ NodeType::WHILE_STATEMENT }, expr{ e }, stmt{ s }
{
}


// Loop
joj::Loop::Loop()
    : Statement(NodeType::LOOP_STATEMENT)
{
}