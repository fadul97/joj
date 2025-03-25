#include "joj/utils/json_parser.h"

#include <iostream>

void joj::JsonParser::hello_world()
{
    std::cout << "Hello, Json Parser!" << std::endl;
}

joj::JsonValue joj::JsonParser::parse(const std::string& json)
{
    return JsonValue();
}