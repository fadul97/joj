#ifndef _JOJ_HPP
#define _JOJ_HPP

#include "lft/fixed_vector.hpp"
#include "lft/string.hpp"

namespace joj {

struct MainArgs {
    lft::FixedVector<lft::String> argv{};
};

int main(MainArgs const& args);

} // namespace joj

#endif // _JOJ_HPP
