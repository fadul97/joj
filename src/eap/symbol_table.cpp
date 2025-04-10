#include "joj/eap/symbol_table.h"

joj::SymbolTable::SymbolTable()
    : table{ std::unordered_map<std::string, Token>{} }, prev{ nullptr }
{
}

joj::SymbolTable::SymbolTable(SymbolTable* st)
    : table{ std::unordered_map<std::string, Token>{} }, prev{ st }
{
}

joj::SymbolTable::~SymbolTable()
{
}


b8 joj::SymbolTable::insert(std::string s, Token token)
{
    const auto& [pos, success] = table.insert({ s, token });
    return success;
}

joj::Token* joj::SymbolTable::find(std::string s)
{
    for (SymbolTable* st = this; st != nullptr; st = st->prev)
    {
        auto found = st->table.find(s);
        if (found != st->table.end())
            return &found->second;
    }

    return nullptr;
}
