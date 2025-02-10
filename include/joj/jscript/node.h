#ifndef _JOJ_NODE_H
#define _JOJ_NODE_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "core/defines.h"

#include "node_type.h"
#include "expression_type.h"
#include "token.h"

namespace joj
{
    class JAPI Node
    {
    public:
        Node();
        Node(NodeType ntype);

    protected:
        NodeType node_type;
    };

    class JAPI Statement : public Node
    {
    public:
        Statement();
        Statement(NodeType ntype);
    };

    class JAPI Expression : public Node
    {
    public:
        Expression(Token* t);
        Expression(NodeType ntype, ExpressionType etype, Token* t);

        std::string get_token_lexeme();
        ExpressionType get_expr_type();

    protected:
        ExpressionType expr_type;
        Token* token;
    };

    class JAPI Constant : public Expression
    {
    public:
        Constant(ExpressionType etype, Token* t);
    };

    class JAPI Identifier : public Expression
    {
    public:
        Identifier(ExpressionType etype, Token* t);
    };

    class JAPI Logical : public Expression
    {
    public:
        Logical(Token* t, Expression* e1, Expression* e2);

    private:
        Expression* expr1;
        Expression* expr2;
    };

    class JAPI Relational : public Expression
    {
    public:
        Relational(Token* t, Expression* e1, Expression* e2);

    private:
        Expression* expr1;
        Expression* expr2;
    };

    class JAPI Arithmetic : public Expression
    {
    public:
        Arithmetic(ExpressionType etype, Token* t, Expression* e1, Expression* e2);

    private:
        Expression* expr1;
        Expression* expr2;
    };

    class JAPI UnaryExpr : public Expression
    {
    public:
        UnaryExpr(ExpressionType etype, Token* t, Expression* e);

    private:
        Expression* expr;
    };

    class JAPI Seq : public Statement
    {
    public:
        Seq(Statement* s, Statement* ss);

    private:
        Statement* stmt;
        Statement* stmts;
    };

    class JAPI Assignment : public Statement
    {
    public:
        Assignment(Expression* i, Expression* e);

    private:
        Expression* id;
        Expression* expr;
    };

    class JAPI If : public Statement
    {
    public:
        If(Expression* e, Statement* s);

    private:
        Expression* expr;
        Statement* stmt;
    };

    class JAPI While : public Statement
    {
    public:
        While(Expression* e, Statement* s);

    private:
        Expression* expr;
        Statement* stmt;
    };

    class JAPI Loop : public Statement
    {
    public:
        Loop();
    };
}

#endif // _JOJ_NODE_H