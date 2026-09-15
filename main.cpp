#include "joj/joj.hpp"

int main(int argc, char** argv)
{
    joj::Args const args = {
        .argc = argc,
        .argv = argv
    };

    return joj::main(args);
}
