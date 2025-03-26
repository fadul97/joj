#include "joj/test/jtest_macros.h"

#undef JOJ_DEBUG_MODE

#include "joj/utils/json_lexer.h"
#include "joj/utils/json_parser.h"
#include <string>
#include "joj/core/logger.h"
#include <print>

JOJ_TEST(JSON_TEST0_Lexer)
{
    std::string json = R"(
        {
            "name": "Cube",
            "vertices": [0, 1, 2]
        }
    )";

#if JOJ_DEBUG_MODE
    std::printf("\n");
#endif
    joj::JsonLexer lexer(json);
    joj::JsonToken token = lexer.next_token();

    assert(lexer.get_error_count() == 0);
}

JOJ_TEST(JSON_TEST1_Lexer)
{
    std::string json = R"(
        {
            "name": "Cube",
            "vertices": [0, 1, 2]
        }
    )";

#if JOJ_DEBUG_MODE
    std::printf("\n");
#endif
    joj::JsonLexer lexer(json);
    joj::JsonToken token = lexer.next_token();

    assert(lexer.get_error_count() == 0);
}

JOJ_TEST(JSON_TEST2_Lexer)
{
    std::string json = R"(
        {
            "name": "Cube",
            "vertices": [0, 1, 2],
            "visible": true
        }
    )";

#if JOJ_DEBUG_MODE
    std::printf("\n");
#endif
    joj::JsonLexer lexer(json);
    joj::JsonToken token = lexer.next_token();

    assert(lexer.get_error_count() == 0);
}

JOJ_TEST(JSON_TEST3_Lexer)
{
    std::string json = R"(
        {
            "name": "Cube",
            "vertices": [0, 1, 2],
            "visible": true
        }
    )";

#if JOJ_DEBUG_MODE
    std::printf("\n");
#endif
    joj::JsonLexer lexer(json);
    joj::JsonToken token = lexer.next_token();

    assert(lexer.get_error_count() == 0);
}

JOJ_TEST(JSON_TEST4_Lexer)
{
    std::string json = R"(
        {
            "name": "Cube",
            "vertices": [0, 1, 2],
            "visible": true,
            "position": { "x": 1.0, "y": 2.0, "z": 3.0 }
        }
    )";

#if JOJ_DEBUG_MODE
    std::printf("\n");
#endif
    joj::JsonLexer lexer(json);
    joj::JsonToken token = lexer.next_token();

    assert(lexer.get_error_count() == 0);
}

JOJ_TEST(JSON_TEST_1)
{
    std::string json = R"(
        {
            "name": "Cube",
            "vertices": [0, 1, 2]
        }
    )";

#if JOJ_DEBUG_MODE
    std::printf("\n");
#endif
    joj::JsonParser parser(json);
    joj::JsonValue result = parser.parse();

    assert(parser.get_error_count() == 0);
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

#if JOJ_DEBUG_MODE
    std::printf("\n");
#endif
    joj::JsonParser parser(json);
    joj::JsonValue result = parser.parse();

    assert(parser.get_error_count() == 0);
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

#if JOJ_DEBUG_MODE
    std::printf("\n");
#endif
    joj::JsonParser parser(json);
    joj::JsonValue result = parser.parse();

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

#if JOJ_DEBUG_MODE
    std::printf("\n");
#endif
    joj::JsonParser parser(json);
    joj::JsonValue result = parser.parse();

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

#if JOJ_DEBUG_MODE
    std::printf("\n");
#endif
    joj::JsonParser parser(json);
    joj::JsonValue root = parser.parse();

    assert(root.is_object());

    // Verificando a seção "asset"
    assert(root["asset"].is_object());
    assert(root["asset"]["generator"].is_string());
    assert(root["asset"]["generator"].as_string() == "Khronos glTF Blender I/O v4.3.47");
    assert(root["asset"]["version"].is_string());
    assert(root["asset"]["version"].as_string() == "2.0");

    // Verificando a chave "scene0"
    assert(root["scene0"].is_number());
    assert(root["scene0"].as_int() == 0);

    // Verificando "scenes1"
    assert(root["scenes1"].is_array());
    assert(root["scenes1"].size() == 1);
    assert(root["scenes1"][0].is_object());
    assert(root["scenes1"][0]["name"].as_string() == "Scene2");
    assert(root["scenes1"][0]["nodes0"].is_array());
    assert(root["scenes1"][0]["nodes0"].size() == 1);
    assert(root["scenes1"][0]["nodes0"][0].as_int() == 0);

    // Verificando "nodes1"
    assert(root["nodes1"].is_array());
    assert(root["nodes1"].size() == 1);
    assert(root["nodes1"][0].is_object());
    assert(root["nodes1"][0]["mesh0"].as_int() == 0);
    assert(root["nodes1"][0]["name0"].as_string() == "Triangle");

    // Verificando "meshes"
    assert(root["meshes"].is_array());
    assert(root["meshes"].size() == 1);
    assert(root["meshes"][0].is_object());
    assert(root["meshes"][0]["name1"].as_string() == "Plane");
    assert(root["meshes"][0]["primitives"].is_array());
    assert(root["meshes"][0]["primitives"].size() == 1);
    assert(root["meshes"][0]["primitives"][0].is_object());
    assert(root["meshes"][0]["primitives"][0]["attributes"].is_object());
    assert(root["meshes"][0]["primitives"][0]["attributes"]["POSITION"].as_int() == 0);
    assert(root["meshes"][0]["primitives"][0]["attributes"]["NORMAL"].as_int() == 1);
    assert(root["meshes"][0]["primitives"][0]["attributes"]["TEXCOORD_0"].as_int() == 2);
    assert(root["meshes"][0]["primitives"][0]["indices"].as_int() == 3);

    // Verificando "accessors"
    assert(root["accessors"].is_array());
    assert(root["accessors"].size() == 4);
    assert(root["accessors"][0].is_object());
    assert(root["accessors"][0]["bufferView"].as_int() == 0);
    assert(root["accessors"][0]["componentType"].as_int() == 5126);
    assert(root["accessors"][0]["count"].as_int() == 3);
    assert(root["accessors"][0]["max"].is_array());
    assert(root["accessors"][0]["max"].size() == 3);
    assert(root["accessors"][0]["max"][0].as_int() == 1);
    assert(root["accessors"][0]["max"][1].as_int() == 0);
    assert(root["accessors"][0]["max"][2].as_int() == 1);
    assert(root["accessors"][0]["min"].is_array());
    assert(root["accessors"][0]["min"].size() == 3);
    assert(root["accessors"][0]["min"][0].as_int() == -1);
    assert(root["accessors"][0]["min"][1].as_int() == 0);
    assert(root["accessors"][0]["min"][2].as_int() == -1);
    assert(root["accessors"][0]["type"].as_string() == "VEC3");

    // Verificando "buffers"
    assert(root["buffers"].is_array());
    assert(root["buffers"].size() == 1);
    assert(root["buffers"][0]["byteLength"].as_int() == 104);
    assert(root["buffers"][0]["uri"].as_string() == "Triangle.bin");
    
    assert(parser.get_error_count() == 0);
}

JOJ_TEST(JSON_TEST_GLTF_CUBE)
{
    std::string json = R"(
        {
            "asset":{
                "generator":"Khronos glTF Blender I/O v4.3.47",
                "version":"2.0"
            },
            "scene":0,
            "scenes":[
                {
                    "name":"Scene",
                    "nodes":[
                        0
                    ]
                }
            ],
            "nodes":[
                {
                    "mesh":0,
                    "name":"Cube"
                }
            ],
            "meshes":[
                {
                    "name":"Cube.001",
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
                    "count":24,
                    "max":[
                        1,
                        1,
                        1
                    ],
                    "min":[
                        -1,
                        -1,
                        -1
                    ],
                    "type":"VEC3"
                },
                {
                    "bufferView":1,
                    "componentType":5126,
                    "count":24,
                    "type":"VEC3"
                },
                {
                    "bufferView":2,
                    "componentType":5126,
                    "count":24,
                    "type":"VEC2"
                },
                {
                    "bufferView":3,
                    "componentType":5123,
                    "count":36,
                    "type":"SCALAR"
                }
            ],
            "bufferViews":[
                {
                    "buffer":0,
                    "byteLength":288,
                    "byteOffset":0,
                    "target":34962
                },
                {
                    "buffer":0,
                    "byteLength":288,
                    "byteOffset":288,
                    "target":34962
                },
                {
                    "buffer":0,
                    "byteLength":192,
                    "byteOffset":576,
                    "target":34962
                },
                {
                    "buffer":0,
                    "byteLength":72,
                    "byteOffset":768,
                    "target":34963
                }
            ],
            "buffers":[
                {
                    "byteLength":840,
                    "uri":"Cube.bin"
                }
            ]
        }
    )";

#if JOJ_DEBUG_MODE
    std::printf("\n");
#endif
    joj::JsonParser parser(json);
    joj::JsonValue root = parser.parse();

    // Acessa o objeto "asset"
    assert(root["asset"].is_object());
    assert(root["asset"]["generator"].as_string() == "Khronos glTF Blender I/O v4.3.47");
    assert(root["asset"]["version"].as_string() == "2.0");

    // Acessa o "scene"
    assert(root["scene"].as_int() == 0);

    // Acessa o array "scenes"
    assert(root["scenes"].size() == 1);
    assert(root["scenes"][0].is_object());
    assert(root["scenes"][0]["name"].as_string() == "Scene");
    assert(root["scenes"][0]["nodes"].size() == 1);
    assert(root["scenes"][0]["nodes"][0].as_int() == 0);

    // Acessa o array "nodes"
    assert(root["nodes"].size() == 1);
    assert(root["nodes"][0].is_object());
    assert(root["nodes"][0]["mesh"].as_int() == 0);
    assert(root["nodes"][0]["name"].as_string() == "Cube");

    // Acessa o array "meshes"
    assert(root["meshes"].size() == 1);
    assert(root["meshes"][0].is_object());
    assert(root["meshes"][0]["name"].as_string() == "Cube.001");
    assert(root["meshes"][0]["primitives"].size() == 1);
    assert(root["meshes"][0]["primitives"][0].is_object());
    assert(root["meshes"][0]["primitives"][0]["attributes"].is_object());
    assert(root["meshes"][0]["primitives"][0]["attributes"]["POSITION"].as_int() == 0);
    assert(root["meshes"][0]["primitives"][0]["attributes"]["NORMAL"].as_int() == 1);
    assert(root["meshes"][0]["primitives"][0]["attributes"]["TEXCOORD_0"].as_int() == 2);
    assert(root["meshes"][0]["primitives"][0]["indices"].as_int() == 3);

    // Acessa o array "accessors"
    assert(root["accessors"].size() == 4);
    assert(root["accessors"][0].is_object());
    assert(root["accessors"][0]["bufferView"].as_int() == 0);
    assert(root["accessors"][0]["componentType"].as_int() == 5126);
    assert(root["accessors"][0]["count"].as_int() == 24);
    assert(root["accessors"][0]["max"].size() == 3);
    assert(root["accessors"][0]["min"].size() == 3);
    assert(root["accessors"][0]["type"].as_string() == "VEC3");

    assert(root["accessors"][1].is_object());
    assert(root["accessors"][1]["bufferView"].as_int() == 1);
    assert(root["accessors"][1]["componentType"].as_int() == 5126);
    assert(root["accessors"][1]["count"].as_int() == 24);
    assert(root["accessors"][1]["type"].as_string() == "VEC3");

    assert(root["accessors"][2].is_object());
    assert(root["accessors"][2]["bufferView"].as_int() == 2);
    assert(root["accessors"][2]["componentType"].as_int() == 5126);
    assert(root["accessors"][2]["count"].as_int() == 24);
    assert(root["accessors"][2]["type"].as_string() == "VEC2");

    assert(root["accessors"][3].is_object());
    assert(root["accessors"][3]["bufferView"].as_int() == 3);
    assert(root["accessors"][3]["componentType"].as_int() == 5123);
    assert(root["accessors"][3]["count"].as_int() == 36);
    assert(root["accessors"][3]["type"].as_string() == "SCALAR");

    // Acessa o array "bufferViews"
    assert(root["bufferViews"].size() == 4);
    assert(root["bufferViews"][0].is_object());
    assert(root["bufferViews"][0]["buffer"].as_int() == 0);
    assert(root["bufferViews"][0]["byteLength"].as_int() == 288);
    assert(root["bufferViews"][0]["byteOffset"].as_int() == 0);
    assert(root["bufferViews"][0]["target"].as_int() == 34962);

    assert(root["bufferViews"][1].is_object());
    assert(root["bufferViews"][1]["buffer"].as_int() == 0);
    assert(root["bufferViews"][1]["byteLength"].as_int() == 288);
    assert(root["bufferViews"][1]["byteOffset"].as_int() == 288);
    assert(root["bufferViews"][1]["target"].as_int() == 34962);

    assert(root["bufferViews"][2].is_object());
    assert(root["bufferViews"][2]["buffer"].as_int() == 0);
    assert(root["bufferViews"][2]["byteLength"].as_int() == 192);
    assert(root["bufferViews"][2]["byteOffset"].as_int() == 576);
    assert(root["bufferViews"][2]["target"].as_int() == 34962);

    assert(root["bufferViews"][3].is_object());
    assert(root["bufferViews"][3]["buffer"].as_int() == 0);
    assert(root["bufferViews"][3]["byteLength"].as_int() == 72);
    assert(root["bufferViews"][3]["byteOffset"].as_int() == 768);
    assert(root["bufferViews"][3]["target"].as_int() == 34963);

    // Acessa o array "buffers"
    assert(root["buffers"].size() == 1);
    assert(root["buffers"][0].is_object());
    assert(root["buffers"][0]["byteLength"].as_int() == 840);
    assert(root["buffers"][0]["uri"].as_string() == "Cube.bin");
    
    assert(parser.get_error_count() == 0);
}

JOJ_TEST(JSON_TEST_PARSER_GLTF_SUZANNE)
{
    std::string json = R"(
        {
            "asset":{
                "generator":"Khronos glTF Blender I/O v4.4.55",
                "version":"2.0"
            },
            "scene":0,
            "scenes":[
                {
                    "name":"Scene",
                    "nodes":[
                        0
                    ]
                }
            ],
            "nodes":[
                {
                    "mesh":0,
                    "name":"Suzanne"
                }
            ],
            "meshes":[
                {
                    "name":"Suzanne",
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
                    "count":1966,
                    "max":[
                        1.3671875,
                        0.984375,
                        0.8515625
                    ],
                    "min":[
                        -1.3671875,
                        -0.984375,
                        -0.8515625
                    ],
                    "type":"VEC3"
                },
                {
                    "bufferView":1,
                    "componentType":5126,
                    "count":1966,
                    "type":"VEC3"
                },
                {
                    "bufferView":2,
                    "componentType":5126,
                    "count":1966,
                    "type":"VEC2"
                },
                {
                    "bufferView":3,
                    "componentType":5123,
                    "count":2904,
                    "type":"SCALAR"
                }
            ],
            "bufferViews":[
                {
                    "buffer":0,
                    "byteLength":23592,
                    "byteOffset":0,
                    "target":34962
                },
                {
                    "buffer":0,
                    "byteLength":23592,
                    "byteOffset":23592,
                    "target":34962
                },
                {
                    "buffer":0,
                    "byteLength":15728,
                    "byteOffset":47184,
                    "target":34962
                },
                {
                    "buffer":0,
                    "byteLength":5808,
                    "byteOffset":62912,
                    "target":34963
                }
            ],
            "buffers":[
                {
                    "byteLength":68720,
                    "uri":"Monkey.bin"
                }
            ]
        }
    )";

#if JOJ_DEBUG_MODE
    std::printf("\n");
#endif
    joj::JsonParser parser(json);
    joj::JsonValue root = parser.parse();

    // Acessa o objeto "asset"
    assert(root["asset"].is_object());
    assert(root["asset"]["generator"].as_string() == "Khronos glTF Blender I/O v4.4.55");
    assert(root["asset"]["version"].as_string() == "2.0");

    // Acessa o "scene"
    assert(root["scene"].as_int() == 0);

    // Acessa o array "scenes"
    assert(root["scenes"].size() == 1);
    assert(root["scenes"][0].is_object());
    assert(root["scenes"][0]["name"].as_string() == "Scene");
    assert(root["scenes"][0]["nodes"].size() == 1);
    assert(root["scenes"][0]["nodes"][0].as_int() == 0);

    // Acessa o array "nodes"
    assert(root["nodes"].size() == 1);
    assert(root["nodes"][0].is_object());
    assert(root["nodes"][0]["mesh"].as_int() == 0);
    assert(root["nodes"][0]["name"].as_string() == "Suzanne");

    // Acessa o array "meshes"
    assert(root["meshes"].size() == 1);
    assert(root["meshes"][0].is_object());
    assert(root["meshes"][0]["name"].as_string() == "Suzanne");
    assert(root["meshes"][0]["primitives"].size() == 1);
    assert(root["meshes"][0]["primitives"][0].is_object());
    assert(root["meshes"][0]["primitives"][0]["attributes"].is_object());
    assert(root["meshes"][0]["primitives"][0]["attributes"]["POSITION"].as_int() == 0);
    assert(root["meshes"][0]["primitives"][0]["attributes"]["NORMAL"].as_int() == 1);
    assert(root["meshes"][0]["primitives"][0]["attributes"]["TEXCOORD_0"].as_int() == 2);
    assert(root["meshes"][0]["primitives"][0]["indices"].as_int() == 3);

    // Acessa o array "accessors"
    assert(root["accessors"].size() == 4);
    assert(root["accessors"][0].is_object());
    assert(root["accessors"][0]["bufferView"].as_int() == 0);
    assert(root["accessors"][0]["componentType"].as_int() == 5126);
    assert(root["accessors"][0]["count"].as_int() == 1966);
    assert(root["accessors"][0]["max"].size() == 3);
    assert(root["accessors"][0]["min"].size() == 3);
    assert(root["accessors"][0]["type"].as_string() == "VEC3");

    assert(root["accessors"][1].is_object());
    assert(root["accessors"][1]["bufferView"].as_int() == 1);
    assert(root["accessors"][1]["componentType"].as_int() == 5126);
    assert(root["accessors"][1]["count"].as_int() == 1966);
    assert(root["accessors"][1]["type"].as_string() == "VEC3");

    assert(root["accessors"][2].is_object());
    assert(root["accessors"][2]["bufferView"].as_int() == 2);
    assert(root["accessors"][2]["componentType"].as_int() == 5126);
    assert(root["accessors"][2]["count"].as_int() == 1966);
    assert(root["accessors"][2]["type"].as_string() == "VEC2");

    assert(root["accessors"][3].is_object());
    assert(root["accessors"][3]["bufferView"].as_int() == 3);
    assert(root["accessors"][3]["componentType"].as_int() == 5123);
    assert(root["accessors"][3]["count"].as_int() == 2904);
    assert(root["accessors"][3]["type"].as_string() == "SCALAR");

    // Acessa o array "bufferViews"
    assert(root["bufferViews"].size() == 4);
    assert(root["bufferViews"][0].is_object());
    assert(root["bufferViews"][0]["buffer"].as_int() == 0);
    assert(root["bufferViews"][0]["byteLength"].as_int() == 23592);
    assert(root["bufferViews"][0]["byteOffset"].as_int() == 0);
    assert(root["bufferViews"][0]["target"].as_int() == 34962);

    assert(root["bufferViews"][1].is_object());
    assert(root["bufferViews"][1]["buffer"].as_int() == 0);
    assert(root["bufferViews"][1]["byteLength"].as_int() == 23592);
    assert(root["bufferViews"][1]["byteOffset"].as_int() == 23592);
    assert(root["bufferViews"][1]["target"].as_int() == 34962);

    assert(root["bufferViews"][2].is_object());
    assert(root["bufferViews"][2]["buffer"].as_int() == 0);
    assert(root["bufferViews"][2]["byteLength"].as_int() == 15728);
    assert(root["bufferViews"][2]["byteOffset"].as_int() == 47184);
    assert(root["bufferViews"][2]["target"].as_int() == 34962);

    assert(root["bufferViews"][3].is_object());
    assert(root["bufferViews"][3]["buffer"].as_int() == 0);
    assert(root["bufferViews"][3]["byteLength"].as_int() == 5808);
    assert(root["bufferViews"][3]["byteOffset"].as_int() == 62912);
    assert(root["bufferViews"][3]["target"].as_int() == 34963);

    // Acessa o array "buffers"
    assert(root["buffers"].size() == 1);
    assert(root["buffers"][0].is_object());
    assert(root["buffers"][0]["byteLength"].as_int() == 68720);
    assert(root["buffers"][0]["uri"].as_string() == "Monkey.bin");
    
    assert(parser.get_error_count() == 0);
}

JOJ_TEST(JSON_TEST_GLTF_SUZANNE)
{
    std::string json = R"(
        {
            "asset":{
                "generator":"Khronos glTF Blender I/O v4.4.55",
                "version":"2.0"
            },
            "scene":0,
            "scenes":[
                {
                    "name":"Scene",
                    "nodes":[
                        0
                    ]
                }
            ],
            "nodes":[
                {
                    "mesh":0,
                    "name":"Suzanne"
                }
            ],
            "meshes":[
                {
                    "name":"Suzanne",
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
                    "count":1966,
                    "max":[
                        1.3671875,
                        0.984375,
                        0.8515625
                    ],
                    "min":[
                        -1.3671875,
                        -0.984375,
                        -0.8515625
                    ],
                    "type":"VEC3"
                },
                {
                    "bufferView":1,
                    "componentType":5126,
                    "count":1966,
                    "type":"VEC3"
                },
                {
                    "bufferView":2,
                    "componentType":5126,
                    "count":1966,
                    "type":"VEC2"
                },
                {
                    "bufferView":3,
                    "componentType":5123,
                    "count":2904,
                    "type":"SCALAR"
                }
            ],
            "bufferViews":[
                {
                    "buffer":0,
                    "byteLength":23592,
                    "byteOffset":0,
                    "target":34962
                },
                {
                    "buffer":0,
                    "byteLength":23592,
                    "byteOffset":23592,
                    "target":34962
                },
                {
                    "buffer":0,
                    "byteLength":15728,
                    "byteOffset":47184,
                    "target":34962
                },
                {
                    "buffer":0,
                    "byteLength":5808,
                    "byteOffset":62912,
                    "target":34963
                }
            ],
            "buffers":[
                {
                    "byteLength":68720,
                    "uri":"Monkey.bin"
                }
            ]
        }
    )";

#if JOJ_DEBUG_MODE
    std::printf("\n");
#endif
    joj::JsonParser parser(json);
    joj::JsonValue root = parser.parse();

    // Asset: generator e version
    assert(root["asset"]["generator"].as_string() == "Khronos glTF Blender I/O v4.4.55");
    assert(root["asset"]["version"].as_string() == "2.0");

    // Scene
    assert(root["scene"].as_number() == 0);

    // Verificar o nome da cena e o primeiro nó
    assert(root["scenes"].as_array()[0]["name"].as_string() == "Scene");
    assert(root["scenes"].as_array()[0]["nodes"].as_array()[0].as_number() == 0);
    
    // Nodes
    assert(root["nodes"].as_array()[0]["mesh"].as_number() == 0);
    assert(root["nodes"].as_array()[0]["name"].as_string() == "Suzanne");
    
    // Meshes
    assert(root["meshes"].as_array()[0]["name"].as_string() == "Suzanne");
    
    // Accessors
    assert(root["accessors"].as_array()[0]["bufferView"].as_number() == 0);
    assert(root["accessors"].as_array()[0]["componentType"].as_number() == 5126);
    assert(root["accessors"].as_array()[0]["count"].as_number() == 1966);
    assert(root["accessors"].as_array()[0]["type"].as_string() == "VEC3");
    assert(root["accessors"].as_array()[0]["max"].as_array()[0].as_number() == 1.3671875);
    assert(root["accessors"].as_array()[0]["min"].as_array()[0].as_number() == -1.3671875);
    
    // BufferViews
    assert(root["bufferViews"].as_array()[0]["buffer"].as_number() == 0);
    assert(root["bufferViews"].as_array()[0]["byteLength"].as_number() == 23592);
    assert(root["bufferViews"].as_array()[0]["byteOffset"].as_number() == 0);
    assert(root["bufferViews"].as_array()[0]["target"].as_number() == 34962);
    
    // Buffers
    assert(root["buffers"].as_array()[0]["byteLength"].as_number() == 68720);
    assert(root["buffers"].as_array()[0]["uri"].as_string() == "Monkey.bin");

    assert(parser.get_error_count() == 0);
}

JOJ_TEST(JSON_TEST_OperatorOverload)
{
    std::string json = R"(
        {
            "name": "Game Engine",
            "version": 1.0,
            "features": ["Physics", "Rendering", "AI"],
            "settings": {
                "resolution": {"width": 1920, "height": 1080},
                "fullscreen": true
            }
        }
    )";

    joj::JsonParser parser(json);
    joj::JsonValue root = parser.parse();

#if JOJ_DEBUG_MODE
    std::printf("\n");
    std::cout << "Name: " << root["name"].as_string() << "\n";
    std::cout << "Version: " << root["version"].as_number() << "\n";
    std::cout << "Fullscreen: " << (root["settings"]["fullscreen"].as_bool() ? "Yes" : "No") << "\n";
    
    // Iterando arrays
    std::cout << "Features:\n";
    for (const joj::JsonValue& feature : root["features"].as_array())
    {
        std::cout << "- " << feature.as_string() << "\n";
    }
#endif
    
    assert(parser.get_error_count() == 0);
}

JOJ_TEST(JSON_TEST_ArrayTest_0)
{
    std::string json = R"(
        {
            "array_test": [
                1, 2, 3, 4, 5,

                "apple", "banana", "cherry",

                true, false, true,

                { "key1": "value1", "key2": "value2" },
                { "name": "John", "age": 30 },

                null, null, null,

                [],

                [1, 2, 3, [4, 5, 6], 7],

                { "type": "fruit", "name": "apple", "count": 10 },
                3, "orange",

                1, 2, 3,
                "a", "b",

                "invalid_key": [1, 2, 3]
            ]
        }
    )";

    joj::JsonParser parser(json);
    joj::JsonValue root = parser.parse();

    // std::printf("\n");
    // root.print();

    assert(parser.get_error_count() != 0);
}

JOJ_TEST(JSON_TEST_ArrayTest_1)
{
    std::string json = R"(
        {
            "asset": {
                "version": "2.0"
            },
            "scenes": [
                {
                    "name": "Scene",
                    "nodes": [0]
                }
            ],
            "nodes": [
                {
                    "name": "CubeNode",
                    "mesh": 0,
                    "translation": [0, 0, 0]
                }
            ],
            "meshes": [
                {
                    "primitives": [
                        {
                            "attributes": {
                                "POSITION": 0
                            },
                            "indices": 1
                        }
                    ]
                }
            ],
            "bufferViews": [
                {
                    "buffer": 0,
                    "byteLength": 288,
                    "byteOffset": 0,
                    "target": 34962
                },
                {
                    "buffer": 0,
                    "byteLength": 72,
                    "byteOffset": 288,
                    "target": 34963
                }
            ],
            "buffers": [
                {
                    "byteLength": 360,
                    "uri": "cube.bin"
                }
            ]
        }
    )";

    joj::JsonParser parser(json);
    joj::JsonValue root = parser.parse();

    // std::printf("\n");
    // root.print();

    assert(parser.get_error_count() == 0);
}

int main() {
    joj::JTestSuite::instance().run();
    return 0;
}