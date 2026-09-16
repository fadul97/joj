#include "joj/joj.hpp"

int main(int argc, char** argv)
{
    joj::MainArgs args;
    args.argv.reserve(argc);
    for (int i = 0; i < argc; ++i)
    {
        args.argv[i] = argv[i];
    }

    return joj::main(args);
}
