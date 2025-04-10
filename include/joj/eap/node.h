#ifndef _JOJ_NODE_H
#define _JOJ_NODE_H

#include "joj/core/defines.h"

#include "node_type.h"
#include "expression_type.h"
#include "token.h"

namespace joj
{
    class JOJ_API Node
    {
    public:
        Node();
        Node(NodeType ntype);

    protected:
        NodeType node_type;
    };

    class JOJ_API Statement : public Node
    {
    public:
        Statement();
        Statement(NodeType ntype);
    };

    class JOJ_API Expression : public Node
    {
    public:
        Expression(Token* t);
        Expression(NodeType ntype, ExpressionType etype, Token* t);

        std::string get_lexeme();
        ExpressionType get_type();

    protected:
        ExpressionType expr_type;
        Token* token;
    };

    class JOJ_API Constant : public Expression
    {
    public:
        Constant(ExpressionType etype, Token* t);
    };

    class JOJ_API Identifier : public Expression
    {
    public:
        Identifier(ExpressionType etype, Token* t);
    };

    class JOJ_API Logical : public Expression
    {
    public:
        Logical(Token* t, Expression* e1, Expression* e2);

    private:
        Expression* expr1;
        Expression* expr2;
    };

    class JOJ_API Relational : public Expression
    {
    public:
        Relational(Token* t, Expression* e1, Expression* e2);

    private:
        Expression* expr1;
        Expression* expr2;
    };

    class JOJ_API Arithmetic : public Expression
    {
    public:
        Arithmetic(ExpressionType etype, Token* t, Expression* e1, Expression* e2);

    private:
        Expression* expr1;
        Expression* expr2;
    };

    class JOJ_API UnaryExpr : public Expression
    {
    public:
        UnaryExpr(ExpressionType etype, Token* t, Expression* e);

    private:
        Expression* expr;
    };

    class JOJ_API Seq : public Statement
    {
    public:
        Seq(Statement* s, Statement* ss);

    private:
        Statement* stmt;
        Statement* stmts;
    };

    class JOJ_API Assignment : public Statement
    {
    public:
        Assignment(Expression* i, Expression* e);

    private:
        Expression* id;
        Expression* expr;
    };

    class JOJ_API If : public Statement
    {
    public:
        If(Expression* e, Statement* s);

    private:
        Expression* expr;
        Statement* stmt;
    };

    class JOJ_API While : public Statement
    {
    public:
        While(Expression* e, Statement* s);

    private:
        Expression* expr;
        Statement* stmt;
    };

    class JOJ_API Loop : public Statement
    {
    public:
        Loop();
    };
}

#endif // _JOJ_NODE_H