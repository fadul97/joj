#ifndef _JOJ_PARSER_H
#define _JOJ_PARSER_H

#define JOJ_ENGINE_IMPLEMENTATION
#include "core/defines.h"

#include "core/vector.h"
#include "lexer.h"
#include "symbol_table.h"

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
        void statements();
        void statement();

        void expr();
        void term();
        void fact();
        b8 match(TokenType type);
    };
}

#endif // _JOJ_PARSER_H