#include "joj/test/jtest_macros.h"

#undef JOJ_DEBUG_MODE

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
    joj::JsonValue result = parser.parse();
    
    assert(parser.get_error_count() == 0);
}

int main() {
    joj::JTestSuite::instance().run();
    return 0;
}