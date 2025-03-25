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

    assert(lexer.get_error_count() == 0);
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

    assert(lexer.get_error_count() == 0);
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

    assert(parser.get_error_count() == 0);
}

JOJ_TEST(JSON_TEST_4)
{
    std::string json = R"(
        {
            "name": "Cube",
            "vertices": [0, 1, 2],
            "visible": true,
            "position": { "x": 1.0, "y": 2.0, "z": 3.0 }
        }
    )";

    joj::JsonParser parser(json);
    joj::JsonValue result = parser.parse();
    std::printf("\n");
    result.print();
    std::printf("\n");

    assert(parser.get_error_count() == 0);
}

JOJ_TEST(JSON_TEST_GLTF_TRIANGLE)
{
    std::string json = R"(
        {
            "asset":{
                "generator":"Khronos glTF Blender I/O v4.3.47",
                "version":"2.0"
            },
            "scene0":0,
            "scenes1":[
                {
                    "name":"Scene2",
                    "nodes0":[
                        0
                    ]
                }
            ],
            "nodes1":[
                {
                    "mesh0":0,
                    "name0":"Triangle"
                }
            ],
            "meshes":[
                {
                    "name1":"Plane",
                    "primitives":[
                        {
                            "attributes":{
                                "POSITION":0,
                                "NORMAL":1,
                                "TEXCOORD_0":2
                            },
                            "indices":3
                        }
                    ]
                }
            ],
            "accessors":[
                {
                    "bufferView":0,
                    "componentType":5126,
                    "count":3,
                    "max":[
                        1,
                        0,
                        1
                    ],
                    "min":[
                        -1,
                        0,
                        -1
                    ],
                    "type":"VEC3"
                },
                {
                    "bufferView":1,
                    "componentType":5126,
                    "count":3,
                    "type":"VEC3"
                },
                {
                    "bufferView":2,
                    "componentType":5126,
                    "count":3,
                    "type":"VEC2"
                },
                {
                    "bufferView":3,
                    "componentType":5123,
                    "count":3,
                    "type":"SCALAR"
                }
            ],
            "bufferViews":[
                {
                    "buffer":0,
                    "byteLength":36,
                    "byteOffset":0,
                    "target":34962
                },
                {
                    "buffer":0,
                    "byteLength":36,
                    "byteOffset":36,
                    "target":34962
                },
                {
                    "buffer":0,
                    "byteLength":24,
                    "byteOffset":72,
                    "target":34962
                },
                {
                    "buffer":0,
                    "byteLength":6,
                    "byteOffset":96,
                    "target":34963
                }
            ],
            "buffers":[
                {
                    "byteLength":104,
                    "uri":"Triangle.bin"
                }
            ]
        }
    )";

    joj::JsonParser parser(json);
    joj::JsonValue result = parser.parse();

    assert(parser.get_error_count() == 0);
}


int main() {
    joj::JTestSuite::instance().run();
    return 0;
}