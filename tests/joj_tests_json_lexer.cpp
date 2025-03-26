#include "joj/test/jtest_macros.h"

#undef JOJ_DEBUG_MODE

#include "joj/utils/json_lexer.h"
#include "joj/utils/json_parser.h"
#include <string>
#include "joj/core/logger.h"
#include <iostream>

JOJ_TEST(JSON_TEST0_Lexer)
{
    std::string json = R"(
        {
            "name": "Cube",
            "vertices": [0, 1, 2]
        }
    )";

#if JOJ_DEBUG_MODE
    std::cout << "\n";
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
    std::cout << "\n";
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
    std::cout << "\n";
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
    std::cout << "\n";
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
    std::cout << "\n";
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
    std::cout << "\n";
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
    std::cout << "\n";
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
    std::cout << "\n";
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
    std::cout << "\n";
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
    std::cout << "\n";
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
    std::cout << "\n";
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
    std::cout << "\n";
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
    std::cout << "\n";
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
    std::cout << "\n";
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

    // std::cout << "\n";
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

    // std::cout << "\n";
    // root.print();

    assert(parser.get_error_count() == 0);
}

JOJ_TEST(JSON_TEST_GLTF_ANIM_SIMPLE_CUBE)
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
                    "name":"Cube"
                }
            ],
            "animations":[
                {
                    "channels":[
                        {
                            "sampler":0,
                            "target":{
                                "node":0,
                                "path":"translation"
                            }
                        },
                        {
                            "sampler":1,
                            "target":{
                                "node":0,
                                "path":"rotation"
                            }
                        },
                        {
                            "sampler":2,
                            "target":{
                                "node":0,
                                "path":"scale"
                            }
                        }
                    ],
                    "name":"CubeAction",
                    "samplers":[
                        {
                            "input":4,
                            "interpolation":"LINEAR",
                            "output":5
                        },
                        {
                            "input":6,
                            "interpolation":"STEP",
                            "output":7
                        },
                        {
                            "input":6,
                            "interpolation":"STEP",
                            "output":8
                        }
                    ]
                }
            ],
            "materials":[
                {
                    "doubleSided":true,
                    "name":"Material",
                    "pbrMetallicRoughness":{
                        "baseColorFactor":[
                            0.800000011920929,
                            0.800000011920929,
                            0.800000011920929,
                            1
                        ],
                        "metallicFactor":0,
                        "roughnessFactor":0.5
                    }
                }
            ],
            "meshes":[
                {
                    "name":"Cube",
                    "primitives":[
                        {
                            "attributes":{
                                "POSITION":0,
                                "NORMAL":1,
                                "TEXCOORD_0":2
                            },
                            "indices":3,
                            "material":0
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
                },
                {
                    "bufferView":4,
                    "componentType":5126,
                    "count":100,
                    "max":[
                        4.166666666666667
                    ],
                    "min":[
                        0.041666666666666664
                    ],
                    "type":"SCALAR"
                },
                {
                    "bufferView":5,
                    "componentType":5126,
                    "count":100,
                    "type":"VEC3"
                },
                {
                    "bufferView":6,
                    "componentType":5126,
                    "count":2,
                    "max":[
                        4.166666666666667
                    ],
                    "min":[
                        0.041666666666666664
                    ],
                    "type":"SCALAR"
                },
                {
                    "bufferView":7,
                    "componentType":5126,
                    "count":2,
                    "type":"VEC4"
                },
                {
                    "bufferView":8,
                    "componentType":5126,
                    "count":2,
                    "type":"VEC3"
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
                },
                {
                    "buffer":0,
                    "byteLength":400,
                    "byteOffset":840
                },
                {
                    "buffer":0,
                    "byteLength":1200,
                    "byteOffset":1240
                },
                {
                    "buffer":0,
                    "byteLength":8,
                    "byteOffset":2440
                },
                {
                    "buffer":0,
                    "byteLength":32,
                    "byteOffset":2448
                },
                {
                    "buffer":0,
                    "byteLength":24,
                    "byteOffset":2480
                }
            ],
            "buffers":[
                {
                    "byteLength":2504,
                    "uri":"AnimSimpleCube.bin"
                }
            ]
        }
    )";

    joj::JsonParser parser(json);
    joj::JsonValue root = parser.parse();

    // Asset metadata
    assert(root["asset"].is_object());
    assert(root["asset"].has_key("generator"));
    assert(root["asset"]["generator"].as_string() == "Khronos glTF Blender I/O v4.4.55");
    assert(root["asset"].has_key("version"));
    assert(root["asset"]["version"].as_string() == "2.0");

    // Scene
    assert(root["scene"].as_int() == 0);
    assert(root["scenes"].is_array());
    assert(root["scenes"].size() > 0);
    assert(root["scenes"][0].has_key("nodes"));
    assert(root["scenes"][0]["nodes"].is_array());
    assert(root["scenes"][0]["nodes"].size() > 0);
    assert(root["scenes"][0]["nodes"][0].as_int() == 0);

    // Nodes
    assert(root["nodes"].is_array());
    assert(root["nodes"].size() > 0);
    assert(root["nodes"][0].has_key("mesh"));
    assert(root["nodes"][0]["mesh"].as_int() == 0);
    assert(root["nodes"][0].has_key("name"));
    assert(root["nodes"][0]["name"].as_string() == "Cube");

    // Animations
    assert(root["animations"].is_array());
    assert(root["animations"].size() > 0);
    assert(root["animations"][0].has_key("channels"));
    assert(root["animations"][0]["channels"].size() == 3);
    assert(root["animations"][0]["channels"][0]["target"]["path"].as_string() == "translation");
    assert(root["animations"][0]["channels"][1]["target"]["path"].as_string() == "rotation");
    assert(root["animations"][0]["channels"][2]["target"]["path"].as_string() == "scale");

    // Animations
    assert(root["animations"].is_array());
    assert(root["animations"].size() > 0);

    for (size_t i = 0; i < root["animations"].size(); i++) {
        assert(root["animations"][i].has_key("channels"));
        assert(root["animations"][i]["channels"].is_array());
        assert(root["animations"][i]["channels"].size() > 0); // Garante que há pelo menos um canal

        // Verifica os paths dos canais de animação
        for (size_t j = 0; j < root["animations"][i]["channels"].size(); j++) {
            assert(root["animations"][i]["channels"][j].has_key("target"));
            assert(root["animations"][i]["channels"][j]["target"].has_key("path"));
            std::string path = root["animations"][i]["channels"][j]["target"]["path"].as_string();
            assert(path == "translation" || path == "rotation" || path == "scale"); // Verifica se o path é válido
        }
    }

    // Verifica a primeira animação
    const auto& animation = root["animations"][0];

    assert(animation.has_key("channels"));
    assert(animation["channels"].is_array());
    assert(animation["channels"].size() == 3); // Deve ter 3 canais

    // Verifica o primeiro canal (translation)
    assert(animation["channels"][0].has_key("sampler"));
    assert(animation["channels"][0]["sampler"].as_int() == 0);
    assert(animation["channels"][0].has_key("target"));
    assert(animation["channels"][0]["target"].has_key("node"));
    assert(animation["channels"][0]["target"]["node"].as_int() == 0);
    assert(animation["channels"][0]["target"].has_key("path"));
    assert(animation["channels"][0]["target"]["path"].as_string() == "translation");

    // Verifica o segundo canal (rotation)
    assert(animation["channels"][1].has_key("sampler"));
    assert(animation["channels"][1]["sampler"].as_int() == 1);
    assert(animation["channels"][1].has_key("target"));
    assert(animation["channels"][1]["target"].has_key("node"));
    assert(animation["channels"][1]["target"]["node"].as_int() == 0);
    assert(animation["channels"][1]["target"].has_key("path"));
    assert(animation["channels"][1]["target"]["path"].as_string() == "rotation");

    // Verifica o terceiro canal (scale)
    assert(animation["channels"][2].has_key("sampler"));
    assert(animation["channels"][2]["sampler"].as_int() == 2);
    assert(animation["channels"][2].has_key("target"));
    assert(animation["channels"][2]["target"].has_key("node"));
    assert(animation["channels"][2]["target"]["node"].as_int() == 0);
    assert(animation["channels"][2]["target"].has_key("path"));
    assert(animation["channels"][2]["target"]["path"].as_string() == "scale");

    // Verifica os samplers
    assert(animation.has_key("samplers"));
    assert(animation["samplers"].is_array());
    assert(animation["samplers"].size() == 3); // Deve ter 3 samplers

    // Verifica o primeiro sampler
    assert(animation["samplers"][0].has_key("input"));
    assert(animation["samplers"][0]["input"].as_int() == 4);
    assert(animation["samplers"][0].has_key("interpolation"));
    assert(animation["samplers"][0]["interpolation"].as_string() == "LINEAR");
    assert(animation["samplers"][0].has_key("output"));
    assert(animation["samplers"][0]["output"].as_int() == 5);

    // Verifica o segundo sampler
    assert(animation["samplers"][1].has_key("input"));
    assert(animation["samplers"][1]["input"].as_int() == 6);
    assert(animation["samplers"][1].has_key("interpolation"));
    assert(animation["samplers"][1]["interpolation"].as_string() == "STEP");
    assert(animation["samplers"][1].has_key("output"));
    assert(animation["samplers"][1]["output"].as_int() == 7);

    // Verifica o terceiro sampler
    assert(animation["samplers"][2].has_key("input"));
    assert(animation["samplers"][2]["input"].as_int() == 6);
    assert(animation["samplers"][2].has_key("interpolation"));
    assert(animation["samplers"][2]["interpolation"].as_string() == "STEP");
    assert(animation["samplers"][2].has_key("output"));
    assert(animation["samplers"][2]["output"].as_int() == 8);

    // Materials
    assert(root["materials"].is_array());
    assert(root["materials"].size() > 0);
    assert(root["materials"][0]["name"].as_string() == "Material");
    assert(root["materials"][0]["pbrMetallicRoughness"].has_key("metallicFactor"));
    assert(root["materials"][0]["pbrMetallicRoughness"]["metallicFactor"].as_number() == 0.0);

    // Meshes
    assert(root["meshes"].is_array());
    assert(root["meshes"].size() > 0);
    assert(root["meshes"][0]["name"].as_string() == "Cube");
    assert(root["meshes"][0]["primitives"].is_array());
    assert(root["meshes"][0]["primitives"].size() > 0);

    // Buffers
    assert(root["buffers"].is_array());
    assert(root["buffers"].size() > 0);
    assert(root["buffers"][0]["byteLength"].as_int() == 2504);
    assert(root["buffers"][0]["uri"].as_string() == "AnimSimpleCube.bin");

    assert(root.has_key("accessors"));
    assert(root["accessors"].is_array());

    assert(root.has_key("bufferViews"));
    assert(root["bufferViews"].is_array());

    // Função auxiliar para verificar um accessor
    auto check_accessor = [&](int accessor_index, int expected_count, int expected_bufferView)
    {
        assert(accessor_index >= 0 && accessor_index < root["accessors"].size());
        const auto& accessor = root["accessors"][accessor_index];

        assert(accessor.has_key("count"));
        assert(accessor["count"].as_int() == expected_count);

        assert(accessor.has_key("bufferView"));
        int bufferViewIndex = accessor["bufferView"].as_int();
        assert(bufferViewIndex == expected_bufferView); // Confere se aponta para o bufferView correto

        assert(bufferViewIndex >= 0 && bufferViewIndex < root["bufferViews"].size());
        const auto& bufferView = root["bufferViews"][bufferViewIndex];

        assert(bufferView.has_key("byteLength"));
        int byteLength = bufferView["byteLength"].as_int();
        assert(byteLength >= expected_count * 4); // Supondo floats (4 bytes cada)
    };

    // Garante que há uma animação e que tem os samplers esperados
    assert(root.has_key("animations"));
    assert(root["animations"].is_array());
    assert(root["animations"].size() > 0);

    assert(animation.has_key("samplers"));
    assert(animation["samplers"].is_array());

    const auto& samplers = animation["samplers"];
    assert(samplers.size() == 3); // Deve haver 3 samplers

    // Verifica os accessors baseados no JSON fornecido
    check_accessor(samplers[0]["input"].as_int(), 100, 4);  // Time do translation
    check_accessor(samplers[0]["output"].as_int(), 100, 5); // Translation values

    check_accessor(samplers[1]["input"].as_int(), 2, 6);  // Time do rotation
    check_accessor(samplers[1]["output"].as_int(), 2, 7); // Rotation values

    check_accessor(samplers[2]["input"].as_int(), 2, 6);  // Time do scale (mesmo input do rotation)
    check_accessor(samplers[2]["output"].as_int(), 2, 8); // Scale values
}

JOJ_TEST(JSON_TEST_CubeSRT)
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
                    "name":"Cube"
                }
            ],
            "animations":[
                {
                    "channels":[
                        {
                            "sampler":0,
                            "target":{
                                "node":0,
                                "path":"translation"
                            }
                        },
                        {
                            "sampler":1,
                            "target":{
                                "node":0,
                                "path":"rotation"
                            }
                        },
                        {
                            "sampler":2,
                            "target":{
                                "node":0,
                                "path":"scale"
                            }
                        }
                    ],
                    "name":"CubeAction",
                    "samplers":[
                        {
                            "input":4,
                            "interpolation":"LINEAR",
                            "output":5
                        },
                        {
                            "input":4,
                            "interpolation":"LINEAR",
                            "output":6
                        },
                        {
                            "input":4,
                            "interpolation":"LINEAR",
                            "output":7
                        }
                    ]
                }
            ],
            "materials":[
                {
                    "doubleSided":true,
                    "name":"Material",
                    "pbrMetallicRoughness":{
                        "baseColorFactor":[
                            0.800000011920929,
                            0.800000011920929,
                            0.800000011920929,
                            1
                        ],
                        "metallicFactor":0,
                        "roughnessFactor":0.5
                    }
                }
            ],
            "meshes":[
                {
                    "name":"Cube",
                    "primitives":[
                        {
                            "attributes":{
                                "POSITION":0,
                                "NORMAL":1,
                                "TEXCOORD_0":2
                            },
                            "indices":3,
                            "material":0
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
                },
                {
                    "bufferView":4,
                    "componentType":5126,
                    "count":60,
                    "max":[
                        2.5
                    ],
                    "min":[
                        0.041666666666666664
                    ],
                    "type":"SCALAR"
                },
                {
                    "bufferView":5,
                    "componentType":5126,
                    "count":60,
                    "type":"VEC3"
                },
                {
                    "bufferView":6,
                    "componentType":5126,
                    "count":60,
                    "type":"VEC4"
                },
                {
                    "bufferView":7,
                    "componentType":5126,
                    "count":60,
                    "type":"VEC3"
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
                },
                {
                    "buffer":0,
                    "byteLength":240,
                    "byteOffset":840
                },
                {
                    "buffer":0,
                    "byteLength":720,
                    "byteOffset":1080
                },
                {
                    "buffer":0,
                    "byteLength":960,
                    "byteOffset":1800
                },
                {
                    "buffer":0,
                    "byteLength":720,
                    "byteOffset":2760
                }
            ],
            "buffers":[
                {
                    "byteLength":3480,
                    "uri":"CubeSRT.bin"
                }
            ]
        }
    )";

    joj::JsonParser parser(json);
    joj::JsonValue root = parser.parse();

    // Validação dos accessors
    assert(root.has_key("accessors"));
    assert(root["accessors"].is_array());
    assert(root["accessors"].size() == 8);

    const auto& accessors = root["accessors"];

    // Verifica cada accessor
    assert(accessors[0]["bufferView"].as_int() == 0);
    assert(accessors[0]["componentType"].as_int() == 5126);
    assert(accessors[0]["count"].as_int() == 24);
    assert(accessors[0]["type"].as_string() == "VEC3");
    assert(accessors[0]["max"].is_array() && accessors[0]["max"].size() == 3);
    assert(accessors[0]["max"][0].as_float() == 1.0f);
    assert(accessors[0]["max"][1].as_float() == 1.0f);
    assert(accessors[0]["max"][2].as_float() == 1.0f);
    assert(accessors[0]["min"].is_array() && accessors[0]["min"].size() == 3);
    assert(accessors[0]["min"][0].as_float() == -1.0f);
    assert(accessors[0]["min"][1].as_float() == -1.0f);
    assert(accessors[0]["min"][2].as_float() == -1.0f);

    assert(accessors[1]["bufferView"].as_int() == 1);
    assert(accessors[1]["componentType"].as_int() == 5126);
    assert(accessors[1]["count"].as_int() == 24);
    assert(accessors[1]["type"].as_string() == "VEC3");

    assert(accessors[2]["bufferView"].as_int() == 2);
    assert(accessors[2]["componentType"].as_int() == 5126);
    assert(accessors[2]["count"].as_int() == 24);
    assert(accessors[2]["type"].as_string() == "VEC2");

    assert(accessors[3]["bufferView"].as_int() == 3);
    assert(accessors[3]["componentType"].as_int() == 5123);
    assert(accessors[3]["count"].as_int() == 36);
    assert(accessors[3]["type"].as_string() == "SCALAR");

    assert(accessors[4]["bufferView"].as_int() == 4);
    assert(accessors[4]["componentType"].as_int() == 5126);
    assert(accessors[4]["count"].as_int() == 60);
    assert(accessors[4]["type"].as_string() == "SCALAR");
    assert(accessors[4]["max"].is_array() && accessors[4]["max"].size() == 1);
    assert(accessors[4]["max"][0].as_float() == 2.5f);
    assert(accessors[4]["min"].is_array() && accessors[4]["min"].size() == 1);
    assert(accessors[4]["min"][0].as_float() == 0.041666666666666664f);

    assert(accessors[5]["bufferView"].as_int() == 5);
    assert(accessors[5]["componentType"].as_int() == 5126);
    assert(accessors[5]["count"].as_int() == 60);
    assert(accessors[5]["type"].as_string() == "VEC3");

    assert(accessors[6]["bufferView"].as_int() == 6);
    assert(accessors[6]["componentType"].as_int() == 5126);
    assert(accessors[6]["count"].as_int() == 60);
    assert(accessors[6]["type"].as_string() == "VEC4");

    assert(accessors[7]["bufferView"].as_int() == 7);
    assert(accessors[7]["componentType"].as_int() == 5126);
    assert(accessors[7]["count"].as_int() == 60);
    assert(accessors[7]["type"].as_string() == "VEC3");

    // Validação dos bufferViews
    assert(root.has_key("bufferViews"));
    assert(root["bufferViews"].is_array());
    assert(root["bufferViews"].size() == 8);

    const auto& bufferViews = root["bufferViews"];

    assert(bufferViews[0]["buffer"].as_int() == 0);
    assert(bufferViews[0]["byteLength"].as_int() == 288);
    assert(bufferViews[0]["byteOffset"].as_int() == 0);
    assert(bufferViews[0]["target"].as_int() == 34962);

    assert(bufferViews[1]["buffer"].as_int() == 0);
    assert(bufferViews[1]["byteLength"].as_int() == 288);
    assert(bufferViews[1]["byteOffset"].as_int() == 288);
    assert(bufferViews[1]["target"].as_int() == 34962);

    assert(bufferViews[2]["buffer"].as_int() == 0);
    assert(bufferViews[2]["byteLength"].as_int() == 192);
    assert(bufferViews[2]["byteOffset"].as_int() == 576);
    assert(bufferViews[2]["target"].as_int() == 34962);

    assert(bufferViews[3]["buffer"].as_int() == 0);
    assert(bufferViews[3]["byteLength"].as_int() == 72);
    assert(bufferViews[3]["byteOffset"].as_int() == 768);
    assert(bufferViews[3]["target"].as_int() == 34963);

    assert(bufferViews[4]["buffer"].as_int() == 0);
    assert(bufferViews[4]["byteLength"].as_int() == 240);
    assert(bufferViews[4]["byteOffset"].as_int() == 840);

    assert(bufferViews[5]["buffer"].as_int() == 0);
    assert(bufferViews[5]["byteLength"].as_int() == 720);
    assert(bufferViews[5]["byteOffset"].as_int() == 1080);

    assert(bufferViews[6]["buffer"].as_int() == 0);
    assert(bufferViews[6]["byteLength"].as_int() == 960);
    assert(bufferViews[6]["byteOffset"].as_int() == 1800);

    assert(bufferViews[7]["buffer"].as_int() == 0);
    assert(bufferViews[7]["byteLength"].as_int() == 720);
    assert(bufferViews[7]["byteOffset"].as_int() == 2760);

    // Validação dos buffers
    assert(root.has_key("buffers"));
    assert(root["buffers"].is_array());
    assert(root["buffers"].size() == 1);

    const auto& buffer = root["buffers"][0];

    assert(buffer["byteLength"].as_int() == 3480);
    assert(buffer["uri"].as_string() == "CubeSRT.bin");

    // Supondo que `root` seja o objeto JSON carregado
    assert(root.has_key("accessors"));
    assert(root["accessors"].is_array());
    assert(root["accessors"].size() == 8);

    // Teste do primeiro accessor (VEC3 - posição)
    assert(accessors[0]["bufferView"].as_int() == 0);
    assert(accessors[0]["componentType"].as_int() == 5126);
    assert(accessors[0]["count"].as_int() == 24);
    assert(accessors[0]["type"].as_string() == "VEC3");
    assert(accessors[0]["max"].is_array() && accessors[0]["max"].size() == 3);
    assert(accessors[0]["max"][0].as_float() == 1.0f);
    assert(accessors[0]["max"][1].as_float() == 1.0f);
    assert(accessors[0]["max"][2].as_float() == 1.0f);
    assert(accessors[0]["min"].is_array() && accessors[0]["min"].size() == 3);
    assert(accessors[0]["min"][0].as_float() == -1.0f);
    assert(accessors[0]["min"][1].as_float() == -1.0f);
    assert(accessors[0]["min"][2].as_float() == -1.0f);

    // Teste do segundo accessor (VEC3 - normais)
    assert(accessors[1]["bufferView"].as_int() == 1);
    assert(accessors[1]["componentType"].as_int() == 5126);
    assert(accessors[1]["count"].as_int() == 24);
    assert(accessors[1]["type"].as_string() == "VEC3");

    // Teste do terceiro accessor (VEC2 - coordenadas UV)
    assert(accessors[2]["bufferView"].as_int() == 2);
    assert(accessors[2]["componentType"].as_int() == 5126);
    assert(accessors[2]["count"].as_int() == 24);
    assert(accessors[2]["type"].as_string() == "VEC2");

    // Teste do quarto accessor (SCALAR - índices)
    assert(accessors[3]["bufferView"].as_int() == 3);
    assert(accessors[3]["componentType"].as_int() == 5123);
    assert(accessors[3]["count"].as_int() == 36);
    assert(accessors[3]["type"].as_string() == "SCALAR");

    // Teste do quinto accessor (SCALAR - pesos de animação)
    assert(accessors[4]["bufferView"].as_int() == 4);
    assert(accessors[4]["componentType"].as_int() == 5126);
    assert(accessors[4]["count"].as_int() == 60);
    assert(accessors[4]["type"].as_string() == "SCALAR");
    assert(accessors[4]["max"].is_array() && accessors[4]["max"].size() == 1);
    assert(accessors[4]["max"][0].as_float() == 2.5f);
    assert(accessors[4]["min"].is_array() && accessors[4]["min"].size() == 1);
    assert(accessors[4]["min"][0].as_float() == 0.041666666666666664f);

    // Teste do sexto accessor (VEC3 - posições de esqueleto)
    assert(accessors[5]["bufferView"].as_int() == 5);
    assert(accessors[5]["componentType"].as_int() == 5126);
    assert(accessors[5]["count"].as_int() == 60);
    assert(accessors[5]["type"].as_string() == "VEC3");

    // Teste do sétimo accessor (VEC4 - quaternions de rotação)
    assert(accessors[6]["bufferView"].as_int() == 6);
    assert(accessors[6]["componentType"].as_int() == 5126);
    assert(accessors[6]["count"].as_int() == 60);
    assert(accessors[6]["type"].as_string() == "VEC4");

    // Teste do oitavo accessor (VEC3 - escalas)
    assert(accessors[7]["bufferView"].as_int() == 7);
    assert(accessors[7]["componentType"].as_int() == 5126);
    assert(accessors[7]["count"].as_int() == 60);
    assert(accessors[7]["type"].as_string() == "VEC3");
}

JOJ_TEST(JSON_TEST_BiggerCubeSRT)
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
                    "name":"Cube"
                }
            ],
            "animations":[
                {
                    "channels":[
                        {
                            "sampler":0,
                            "target":{
                                "node":0,
                                "path":"translation"
                            }
                        },
                        {
                            "sampler":1,
                            "target":{
                                "node":0,
                                "path":"rotation"
                            }
                        },
                        {
                            "sampler":2,
                            "target":{
                                "node":0,
                                "path":"scale"
                            }
                        }
                    ],
                    "name":"CubeAction",
                    "samplers":[
                        {
                            "input":4,
                            "interpolation":"LINEAR",
                            "output":5
                        },
                        {
                            "input":4,
                            "interpolation":"LINEAR",
                            "output":6
                        },
                        {
                            "input":4,
                            "interpolation":"LINEAR",
                            "output":7
                        }
                    ]
                }
            ],
            "materials":[
                {
                    "doubleSided":true,
                    "name":"Material",
                    "pbrMetallicRoughness":{
                        "baseColorFactor":[
                            0.800000011920929,
                            0.800000011920929,
                            0.800000011920929,
                            1
                        ],
                        "metallicFactor":0,
                        "roughnessFactor":0.5
                    }
                }
            ],
            "meshes":[
                {
                    "name":"Cube",
                    "primitives":[
                        {
                            "attributes":{
                                "POSITION":0,
                                "NORMAL":1,
                                "TEXCOORD_0":2
                            },
                            "indices":3,
                            "material":0
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
                },
                {
                    "bufferView":4,
                    "componentType":5126,
                    "count":60,
                    "max":[
                        2.5
                    ],
                    "min":[
                        0.041666666666666664
                    ],
                    "type":"SCALAR"
                },
                {
                    "bufferView":5,
                    "componentType":5126,
                    "count":60,
                    "type":"VEC3"
                },
                {
                    "bufferView":6,
                    "componentType":5126,
                    "count":60,
                    "type":"VEC4"
                },
                {
                    "bufferView":7,
                    "componentType":5126,
                    "count":60,
                    "type":"VEC3"
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
                },
                {
                    "buffer":0,
                    "byteLength":240,
                    "byteOffset":840
                },
                {
                    "buffer":0,
                    "byteLength":720,
                    "byteOffset":1080
                },
                {
                    "buffer":0,
                    "byteLength":960,
                    "byteOffset":1800
                },
                {
                    "buffer":0,
                    "byteLength":720,
                    "byteOffset":2760
                }
            ],
            "buffers":[
                {
                    "byteLength":3480,
                    "uri":"BiggerCubeSRT.bin"
                }
            ]
        }
    )";

    joj::JsonParser parser(json);
    joj::JsonValue root = parser.parse();

    assert(root.has_key("asset"));
    assert(root["asset"].has_key("generator"));
    assert(root["asset"]["generator"].as_string() == "Khronos glTF Blender I/O v4.4.55");
    assert(root["asset"].has_key("version"));
    assert(root["asset"]["version"].as_string() == "2.0");

    assert(root.has_key("scene"));
    assert(root["scene"].as_int() == 0);
    assert(root.has_key("scenes"));
    assert(root["scenes"].is_array());
    assert(root["scenes"].size() == 1);
    assert(root["scenes"][0].has_key("name"));
    assert(root["scenes"][0]["name"].as_string() == "Scene");
    assert(root["scenes"][0].has_key("nodes"));
    assert(root["scenes"][0]["nodes"].is_array());
    assert(root["scenes"][0]["nodes"].size() == 1);
    assert(root["scenes"][0]["nodes"][0].as_int() == 0);

    assert(root.has_key("nodes"));
    assert(root["nodes"].is_array());
    assert(root["nodes"].size() == 1);
    assert(root["nodes"][0].has_key("mesh"));
    assert(root["nodes"][0]["mesh"].as_int() == 0);
    assert(root["nodes"][0].has_key("name"));
    assert(root["nodes"][0]["name"].as_string() == "Cube");

    assert(root.has_key("animations"));
    assert(root["animations"].is_array());
    assert(root["animations"].size() == 1);
    assert(root["animations"][0].has_key("name"));
    assert(root["animations"][0]["name"].as_string() == "CubeAction");
    assert(root["animations"][0].has_key("channels"));
    assert(root["animations"][0]["channels"].is_array());
    assert(root["animations"][0]["channels"].size() == 3);
    assert(root["animations"][0]["channels"][0].has_key("sampler"));
    assert(root["animations"][0]["channels"][0]["sampler"].as_int() == 0);
    assert(root["animations"][0]["channels"][0].has_key("target"));
    assert(root["animations"][0]["channels"][0]["target"].has_key("node"));
    assert(root["animations"][0]["channels"][0]["target"]["node"].as_int() == 0);
    assert(root["animations"][0]["channels"][0]["target"].has_key("path"));
    assert(root["animations"][0]["channels"][0]["target"]["path"].as_string() == "translation");

    const auto& accessors = root["accessors"];

    assert(accessors[0]["bufferView"].as_int() == 0);
    assert(accessors[0]["componentType"].as_int() == 5126);
    assert(accessors[0]["count"].as_int() == 24);
    assert(accessors[0]["max"][0].as_float() == 1.0f);
    assert(accessors[0]["max"][1].as_float() == 1.0f);
    assert(accessors[0]["max"][2].as_float() == 1.0f);
    assert(accessors[0]["min"][0].as_float() == -1.0f);
    assert(accessors[0]["min"][1].as_float() == -1.0f);
    assert(accessors[0]["min"][2].as_float() == -1.0f);

    assert(accessors[1]["bufferView"].as_int() == 1);
    assert(accessors[1]["componentType"].as_int() == 5126);
    assert(accessors[1]["count"].as_int() == 24);
    assert(accessors[1]["type"].as_string() == "VEC3");

    assert(accessors[2]["bufferView"].as_int() == 2);
    assert(accessors[2]["componentType"].as_int() == 5126);
    assert(accessors[2]["count"].as_int() == 24);
    assert(accessors[2]["type"].as_string() == "VEC2");

    assert(accessors[3]["bufferView"].as_int() == 3);
    assert(accessors[3]["componentType"].as_int() == 5123);
    assert(accessors[3]["count"].as_int() == 36);
    assert(accessors[3]["type"].as_string() == "SCALAR");

    assert(accessors[4]["bufferView"].as_int() == 4);
    assert(accessors[4]["componentType"].as_int() == 5126);
    assert(accessors[4]["count"].as_int() == 60);
    assert(accessors[4]["max"][0].as_float() == 2.5f);
    assert(accessors[4]["min"][0].as_float() == 0.041666666666666664f);
    assert(accessors[4]["type"].as_string() == "SCALAR");

    assert(accessors[5]["bufferView"].as_int() == 5);
    assert(accessors[5]["componentType"].as_int() == 5126);
    assert(accessors[5]["count"].as_int() == 60);
    assert(accessors[5]["type"].as_string() == "VEC3");

    assert(accessors[6]["bufferView"].as_int() == 6);
    assert(accessors[6]["componentType"].as_int() == 5126);
    assert(accessors[6]["count"].as_int() == 60);
    assert(accessors[6]["type"].as_string() == "VEC4");

    assert(accessors[7]["bufferView"].as_int() == 7);
    assert(accessors[7]["componentType"].as_int() == 5126);
    assert(accessors[7]["count"].as_int() == 60);
    assert(accessors[7]["type"].as_string() == "VEC3");

    const auto& bufferViews = root["bufferViews"];

    assert(bufferViews[0]["buffer"].as_int() == 0);
    assert(bufferViews[0]["byteLength"].as_int() == 288);
    assert(bufferViews[0]["byteOffset"].as_int() == 0);
    assert(bufferViews[0]["target"].as_int() == 34962);

    assert(bufferViews[1]["buffer"].as_int() == 0);
    assert(bufferViews[1]["byteLength"].as_int() == 288);
    assert(bufferViews[1]["byteOffset"].as_int() == 288);
    assert(bufferViews[1]["target"].as_int() == 34962);

    assert(bufferViews[2]["buffer"].as_int() == 0);
    assert(bufferViews[2]["byteLength"].as_int() == 192);
    assert(bufferViews[2]["byteOffset"].as_int() == 576);
    assert(bufferViews[2]["target"].as_int() == 34962);

    assert(bufferViews[3]["buffer"].as_int() == 0);
    assert(bufferViews[3]["byteLength"].as_int() == 72);
    assert(bufferViews[3]["byteOffset"].as_int() == 768);
    assert(bufferViews[3]["target"].as_int() == 34963);

    assert(bufferViews[4]["buffer"].as_int() == 0);
    assert(bufferViews[4]["byteLength"].as_int() == 240);
    assert(bufferViews[4]["byteOffset"].as_int() == 840);

    assert(bufferViews[5]["buffer"].as_int() == 0);
    assert(bufferViews[5]["byteLength"].as_int() == 720);
    assert(bufferViews[5]["byteOffset"].as_int() == 1080);

    assert(bufferViews[6]["buffer"].as_int() == 0);
    assert(bufferViews[6]["byteLength"].as_int() == 960);
    assert(bufferViews[6]["byteOffset"].as_int() == 1800);

    assert(bufferViews[7]["buffer"].as_int() == 0);
    assert(bufferViews[7]["byteLength"].as_int() == 720);
    assert(bufferViews[7]["byteOffset"].as_int() == 2760);

    const auto& buffers = root["buffers"];

    assert(buffers[0]["byteLength"].as_int() == 3480);
    assert(buffers[0]["uri"].as_string() == "BiggerCubeSRT.bin");
}

int main() {
    joj::JTestSuite::instance().run();
    return 0;
}