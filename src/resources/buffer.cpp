#include "joj/resources/buffer.h"

joj::Buffer::Buffer()
    : type(BufferType::UNKNOWN), size(0)
{
}

joj::Buffer::Buffer(const char* filename, const BufferType type, const std::vector<u8>& data)
    : filename(filename), type(type), data(data), size(0)
{
    size = static_cast<u32>(data.size());
}

joj::Buffer::~Buffer()
{
}