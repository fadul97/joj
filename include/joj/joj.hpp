#ifndef _JOJ_HPP
#define _JOJ_HPP

namespace joj {

struct Args {
    int argc{ -1 };
    char** argv{ nullptr };
};

int main(Args const& args);

} // namespace joj

#endif // _JOJ_HPP
