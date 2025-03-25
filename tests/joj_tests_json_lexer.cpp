#include "joj/test/jtest_macros.h"

#include "joj/utils/json_lexer.h"
#include "joj/utils/json_parser.h"
#include <string>
#include "joj/core/logger.h"
#include <print>

JOJ_TEST(JSON_TEST_1)
{
    std::string json = R"(
        {
            "name": "Cube",
            "vertices": [0, 1, 2]
        }
    )";
    joj::JsonLexer lexer(json);

    std::printf("\n");
    joj::JsonToken token;
    while ((token = lexer.next_token()).type != joj::JsonTokenType::EndOfFile)
    {
        // Print token
        std::printf("Token: %s\n", token.value.c_str());
        assert(token.type != joj::JsonTokenType::Error);
    }
}

JOJ_TEST(JSON_TEST_2)
{
    std::string json = R"(
        {
            "name": "Cube",
            "vertices": [0, 1, 2],
            "visible": true
        }
    )";
    joj::JsonLexer lexer(json);

    std::printf("\n");
    joj::JsonToken token;
    while ((token = lexer.next_token()).type != joj::JsonTokenType::EndOfFile)
    {
        // Print token
        std::printf("Token: %s\n", token.value.c_str());
        assert(token.type != joj::JsonTokenType::Error);
    }
}

JOJ_TEST(JSON_TEST_3)
{
    std::string json = R"(
        {
            "name": "Cube",
            "vertices": [0, 1, 2],
            "visible": true
        }
    )";

    joj::JsonParser parser(json);
    joj::JsonValue result = parser.parse();
    std::printf("\n");
    result.print();
    std::printf("\n");
}


int main() {
    joj::JTestSuite::instance().run();
    return 0;
}