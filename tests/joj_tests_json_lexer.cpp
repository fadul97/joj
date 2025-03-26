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
    joj::JsonValue result = parser.parse();
    
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
    joj::JsonValue result = parser.parse();
    
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

    // Asset info
    // std::cout << "Generator: " << root["asset"]["generator"].as_string() << "\n";
    assert(root["asset"]["generator"].as_string() == "Khronos glTF Blender I/O v4.4.55");
    // std::cout << "Version: " << root["asset"]["version"].as_string() << "\n\n";
    assert(root["asset"]["version"].as_string() == "2.0");

    /*
    std::cout << "=== Buffers ===\n";
    // Information about buffers
    if (root["buffers"].is_array())
    {
        for (const joj::JsonValue& buffer : root["buffers"].as_array())
        {
            std::cout << "URI: " << buffer["uri"].as_string() << ", Byte Length: " << buffer["byteLength"].as_number() << "\n";
        }
    }
    
    std::cout << "\n=== BufferViews ===\n";
    // Information about bufferViews
    if (root["bufferViews"].is_array())
    {
        for (const joj::JsonValue& bufferView : root["bufferViews"].as_array())
        {
            std::cout << "Buffer: " << bufferView["buffer"].as_number()
            << ", Byte Length: " << bufferView["byteLength"].as_number()
            << ", Byte Offset: " << bufferView["byteOffset"].as_number()
            << ", Target: " << bufferView["target"].as_number() << "\n";
        }
    }
    
    std::cout << "\n=== Accessors ===\n";
    // Information about accessors
    if (root["accessors"].is_array())
    {
        for (const joj::JsonValue& accessor : root["accessors"].as_array())
        {
            std::cout << "BufferView: " << accessor["bufferView"].as_number()
            << ", Component Type: " << accessor["componentType"].as_number()
            << ", Count: " << accessor["count"].as_number()
            << ", Type: " << accessor["type"].as_string() << "\n";
            
            // Print the values of max and min
            if (accessor["max"].is_array())
            {
                std::cout << "  Max: ";
                for (const joj::JsonValue& max_val : accessor["max"].as_array())
                {
                    std::cout << max_val.as_number() << " ";
                }
                std::cout << "\n";
            }
            
            if (accessor["min"].is_array())
            {
                std::cout << "  Min: ";
                for (const joj::JsonValue& min_val : accessor["min"].as_array())
                {
                    std::cout << min_val.as_number() << " ";
                }
                std::cout << "\n";
            }
        }
    }
    */
    
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

    /*
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
    */

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

    std::printf("\n");
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

    std::printf("\n");
    root.print();

    assert(parser.get_error_count() == 0);
}

int main() {
    joj::JTestSuite::instance().run();
    return 0;
}