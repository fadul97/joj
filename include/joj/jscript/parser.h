#ifndef _JOJ_PARSER_H
#define _JOJ_PARSER_H

#include "joj/core/defines.h"

#include "lexer.h"
#include "symbol_table.h"
#include "node.h"

namespace joj
{
    class JOJ_API Parser
    {
    public:
        Parser(Lexer& lexer);
        ~Parser();

        void start();

        u32 get_error_count() const;

    private:
        Lexer& m_lexer;
        Token* m_lookahead;
        SymbolTable* m_symtable;

        void declaration();

        b8 match(const TokenType type);

        u32 m_errors;
    };
}

#endif // _JOJ_PARSER_H