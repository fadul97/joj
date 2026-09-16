#ifndef _JOJ_HPP
#define _JOJ_HPP

#include "joj/core/containers/fixed_vector.hpp"
#include "joj/core/string/string.hpp"

namespace joj {

struct MainArgs {
    FixedVector<String> argv{};
};

int main(MainArgs const& args);

} // namespace joj

#endif // _JOJ_HPP
