#ifndef _JOJ_PARSER_H
#define _JOJ_PARSER_H

#include "core/defines.h"

#include "core/vector.h"
#include "lexer.h"
#include "symbol_table.h"
#include "node.h"

namespace joj
{
    class JAPI Parser
    {
    public:
        Parser(Lexer& lexer);
        ~Parser();

        void start();

    private:
        Lexer& m_lexer;
        Token* m_lookahead;
        SymbolTable* m_symtable;

        void program();
        void block();
        void declarations();
        void declaration();
        Statement* statements();
        Statement* statement();

        Expression* local();
        Expression* Bool();

        Expression* join();
        Expression* equality();

        Expression* rel();
        Expression* ari();
        Expression* term();
        Expression* unary();
        Expression* factor();

        void expr();
        b8 match(TokenType type);
    };
}

#endif // _JOJ_PARSER_H