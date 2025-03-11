#ifndef _JOJ_SYMBOL_TABLE_H
#define _JOJ_SYMBOL_TABLE_H

#include "joj/core/defines.h"

#include <unordered_map>
#include <string>
#include "token.h"

namespace joj
{
    class JOJ_API SymbolTable
    {
    public:
        SymbolTable();
        SymbolTable(SymbolTable* st);
        ~SymbolTable();

        b8 insert(std::string s, Token token);
        Token* find(std::string s);

    private:
        std::unordered_map<std::string, Token> table;
        SymbolTable* prev;
    };
}

#endif // _JOJ_SYMBOL_TABLE_H