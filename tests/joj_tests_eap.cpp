#include "joj/test/jtest_macros.h"
#include "joj/eap/lexer.h"
#include "joj/eap/parser.h"

JOJ_TEST(Lexer_Var_Declaration)
{
    // std::cout << "\n";
    joj::Lexer lexer("scripts/var_declaration.eap");
    lexer.start();

    joj::Token* token = lexer.scan();
    while (token->type != joj::TokenType::END_OF_FILE)
    {
        /*
        std::cout << "[" << joj::tokentype_to_string(token->type) << "]: "
            << token->lexeme << "    ";

        if (token->type == joj::TokenType::SEMICOLON)
            std::cout << "\n";
        */

        token = lexer.scan();
    }

    assert(lexer.get_error_count() == 0);
}

JOJ_TEST(Parser_Var_Declaration)
{
    joj::Lexer lexer("scripts/var_declaration.eap");
    joj::Parser parser(lexer);

    parser.start();

    assert(parser.get_error_count() == 0);
}

int main()
{
    joj::JTestSuite::instance().run();
    return 0;
}