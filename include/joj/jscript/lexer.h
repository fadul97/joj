#ifndef _JOJ_SCRIPT_H
#define _JOJ_SCRIPT_H

#include "core/defines.h"

#include <string>
#include "core/vector.h"
#include "token.h"
#include <fstream>
#include <unordered_map>

namespace joj
{
    class JAPI Lexer
    {
    public:
        Lexer(const std::string& source);
        ~Lexer();

        void start();

        Token* scan();
        Vector<Token> tokenize();

        const u32 get_line() const;
        const u32 get_errors_count() const;

    private:
        std::string m_source;
        u32 m_index;
        u32 m_lineno;
        char m_peek;
        std::ifstream m_fin;
        Token m_token;
        std::unordered_map<std::string, Token> m_token_table;
        u32 m_errors;
    };

    inline const u32 Lexer::get_line() const
    { return m_lineno; }

    inline const u32 Lexer::get_errors_count() const
    { return m_errors; }
}

#endif // _JOJ_SCRIPT_H