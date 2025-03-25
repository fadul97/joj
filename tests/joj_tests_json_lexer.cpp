#include "joj/test/jtest_macros.h"

#include "joj/utils/json_lexer.h"
#include <string>
#include "joj/core/logger.h"
#include <print>

JOJ_TEST(Parser_Var_Declaration)
{
    std::string json = R"({"name": "Cube", "vertices": [0, 1, 2]})";
    joj::JsonLexer lexer(json);

    joj::JsonToken token;
    while ((token = lexer.next_token()).type != joj::JsonTokenType::EndOfFile)
    {
        // Print token
        std::printf("\nToken: %s", token.value.c_str());
        assert(token.type != joj::JsonTokenType::Error);
    }
}

int main() {
    joj::JTestSuite::instance().run();
    return 0;
}