#include "joj/jscript/lexer.h"
#include "joj/test/jtest_macros.h"

JOJ_TEST(Var_Declaration)
{
    std::cout << "\n";
    joj::Lexer lexer("scripts/var_declaration.jsc");
    lexer.start();

    joj::Token* token = lexer.scan();
    while (token->type != joj::TokenType::END_OF_FILE)
    {
        std::cout << "[" << joj::tokentype_to_string(token->type) << "]: "
            << token->lexeme << "    ";

        if (token->type == joj::TokenType::SEMICOLON)
            std::cout << "\n";

        token = lexer.scan();
    }

    assert(lexer.get_errors_count() == 0);
}

JOJ_TEST(Simple_Main)
{
    std::cout << "\n";
    joj::Lexer lexer("scripts/simple_main.jsc");
    lexer.start();

    joj::Token* token = lexer.scan();
    while (token->type != joj::TokenType::END_OF_FILE)
    {
        std::cout << "[" << joj::tokentype_to_string(token->type) << "]: "
            << token->lexeme << "    ";

        if (token->type == joj::TokenType::SEMICOLON)
            std::cout << "\n";

        token = lexer.scan();
    }

    assert(lexer.get_errors_count() == 0);
}

int main()
{
    joj::JTestSuite::instance().run();
    return 0;
}