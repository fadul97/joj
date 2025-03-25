#ifndef _JOJ_GLTF_IMPORTER_H
#define _JOJ_GLTF_IMPORTER_H

#include "joj/core/defines.h"

#include "joj/core/error_code.h"
#include <vector>
#include "joj/core/math/vector3.h"
namespace joj
{
    struct JOJ_API GLTFVertex
    {
        Vector3 pos;
        Vector3 normal;
    };

    class JOJ_API GLTFImporter
    {
    public:
        GLTFImporter();
        GLTFImporter(const char* filename);
        ~GLTFImporter();

        ErrorCode load();

        std::vector<GLTFVertex> get_vertices();

    private:
        const char* m_gltf_filename;
        const char* m_bin_filename;

        std::vector<u8> m_binary_data;

        b8 parse_json();
    };
}

#endif // _JOJ_GLTF_IMPORTER_H