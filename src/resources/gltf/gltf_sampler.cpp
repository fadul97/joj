#include "joj/resources/gltf/gltf_sampler.h"

joj::GLTFSampler::GLTFSampler()
    : interpolation(InterpolationType::UNKNOWN)
    , input(-1)
    , output(-1)
{
}

joj::GLTFSampler::~GLTFSampler()
{
}